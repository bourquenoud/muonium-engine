/*
 * Renderer3D.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Renderer3D.hpp"

namespace ue
{

  void Renderer3D::RenderFullFrame()
  {
    //Clear the depth buffer, clearing the frame buffer is not necessary
    clearDepthBuffer();
    clearFrameBufferGrid((Colour){0xFFFFFFFF}, (Colour){0xFFCCCCCC}, 12);

    //Works object by object
    for(uint32_t i = 0; i < objectNumber; i++)
      {
        renderObject(objectList[i]);
      }
  }


  void Renderer3D::renderObject(Poly o)
  {
    for(uint32_t i = 0; i < o.faceCount; i++)
      {

        //TODO : make it cleaner
        //Convert to camera perspective and compute the texture vertices correction
        Vector3 projVertexA, projVertexB, projVertexC;
        Triangle projTriangle = { &projVertexA, &projVertexB, &projVertexC };

        *(projTriangle.va) = camera.toRaster(*(o.faces[i].va) + o.position);
        *(projTriangle.vb) = camera.toRaster(*(o.faces[i].vb) + o.position);
        *(projTriangle.vc) = camera.toRaster(*(o.faces[i].vc) + o.position);

        //Don't process triangles behind the camera clipping plane (z is reversed)
        if(projTriangle.va->z > R(0.0) && projTriangle.vb->z > R(0.0) && projTriangle.vc->z > R(0.0))
          continue;

        //Don't process triangles that are too far
        //        if(projTriangle.va->z > camera.far && projTriangle.vb->z > camera.far && projTriangle.vc->z > camera.far)
        //          continue;

        //Process the bounding rectangle
        Vector2 maxCorner = Vector2(
            Real::round(Real::max(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x)),
            Real::round(Real::max(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y)));

        Vector2 minCorner = Vector2(
            Real::round(Real::min(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x)),
            Real::round(Real::min(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y)));

#if UE_CONFIG_ENABLE_TEXTURE == true
        //Copy the texture vectors pointers
        projTriangle.vta = o.faces[i].vta;
        projTriangle.vtb = o.faces[i].vtb;
        projTriangle.vtc = o.faces[i].vtc;
#endif

#if UE_CONFIG_ENABLE_NORMAL == true
        //Copy the normal vectors
        projTriangle.vna = o.faces[i].vna;
        projTriangle.vna = o.faces[i].vnb;
        projTriangle.vna = o.faces[i].vnc;
#endif

#if UE_CONFIG_FAST_CLIPPING == true
        //Skip the triangle if at least one corner is outside the frame
        if(maxCorner.x > (Real)camera.width || maxCorner.y > (Real)camera.height
            || minCorner.x < R(0.0) || minCorner.y < R(0.0))
          continue;
#else
        //TODO: Implement the clipping algorithm

#endif

        renderTriangle(projTriangle, minCorner, maxCorner);
      }
  }

  void Renderer3D::renderTriangle(Triangle t,Vector2 minCorner,Vector2 maxCorner)
  {
    //Compute signed the area
    Real area = edgeFunction(*(t.va), *(t.vb), *(t.vc));

    //If the area if negative the triangle is seen for the back
    // TODO: add the option to disable back culling
    if (area <= R(0))
      return;

    area = R(1)/area;

    //Pre-compute factors for the triangle drawing
    Real k[3][3];
    k[0][0] = t.vb->y - t.va->y;
    k[0][1] = t.va->x - t.vb->x;
    k[0][2] = (t.va->y * t.vb->x) - (t.va->x * t.vb->y);

    k[1][0] = t.vc->y - t.vb->y;
    k[1][1] = t.vb->x - t.vc->x;
    k[1][2] = (t.vb->y * t.vc->x) - (t.vb->x * t.vc->y);

    k[2][0] = t.va->y - t.vc->y;
    k[2][1] = t.vc->x - t.va->x;
    k[2][2] = (t.vc->y * t.va->x) - (t.vc->x * t.va->y);

    Real light = Real::min(computeLight(t), R(1.0));
    Colour col;
    col.colour.r = (uint8_t)((Real)0x7F * light);
    col.colour.g = (uint8_t)((Real)0x1F * light);
    col.colour.b = (uint8_t)((Real)0x80 * light);
    col.colour.a = 0xFF;

    //Starting point barycentric coordinates
    Real startW0 = k[0][0]*minCorner.x + k[0][1]*minCorner.y + k[0][2];
    Real startW1 = k[1][0]*minCorner.x + k[1][1]*minCorner.y + k[1][2];
    Real startW2 = k[2][0]*minCorner.x + k[2][1]*minCorner.y + k[2][2];

    //Scan in the x direction TODO: scan in the y direction
    for(uint16_t y = (uint32_t)minCorner.y; y <= (uint16_t)maxCorner.y; y++)
      {
        Real w0 = startW0;
        Real w1 = startW1;
        Real w2 = startW2;

        for(uint16_t x = (uint32_t)minCorner.x; x <= (uint16_t)maxCorner.x; x++)
          {

            //TODO: use a | sign bit computation instead
            if(w0 >= R(0) && w1 >= R(0) && w2 >= R(0))
              {
                Real z = w0 * t.vc->z + w1 * t.va->z + w2 * t.vb->z;
                z *= area;
                uint32_t i = x+y*camera.width;
                //Check the depth and draw if closer
                //FIXME Remove the abs
                if(z > depthBuffer[i])
                  {
                    depthBuffer[i] = z;
                    //XXX THIS IS A TEMPORARY TEST XXX
                    frameBuffer[i] = col;
                    //XXX ************************ XXX
                  }
              }
            w0 += k[0][0];
            w1 += k[1][0];
            w2 += k[2][0];
          }

        //Move on y
        startW0 += k[0][1];
        startW1 += k[1][1];
        startW2 += k[2][1];

      }

  }

  Real Renderer3D::computeLight(Triangle triangle)
  {
    Real light;
    Vector3 a,b,s1;

    //Get the direction vector
    a = *triangle.vb - *triangle.va;
    b = *triangle.vc - *triangle.va;

    //XXX optimisation in progress
    //Normalise the vectors before (Will overflow in Fixed32 otherwise)
    //Calculate the normal
    s1 = a.normalise().cross(b.normalise());

    //Normalise the vectors
    s1 = s1.normalise();

    //Calculate the dot product
    light = Real::max(R(0.0),(-sun.direction)*s1); //Prevent negative light

    //Compute the directional light
    light *= sun.intensity;

    //Add ambient light
    light += ambientLight;

    return light;
  }

  void Renderer3D::clearDepthBuffer()
  {
    for(uint32_t i = 0; i < depthBuffer.width * depthBuffer.height; i++)
      {
        depthBuffer[i] = UE_REAL_MIN;
      }
  }

  void Renderer3D::clearFrameBuffer(Colour col)
  {
    for(uint32_t i = 0; i < frameBuffer.width * frameBuffer.height; i++)
      {
        frameBuffer[i] = col;
      }
  }

  void Renderer3D::clearFrameBufferGrid(Colour col1, Colour col2, uint32_t size)
  {
    for(uint32_t i = 0; i < frameBuffer.width * frameBuffer.height; i++)
      {
        uint32_t x = i%frameBuffer.width;
        uint32_t y = i/frameBuffer.width;

        frameBuffer[i] = (((x/size) & 1)^((y/size) & 1))?col1:col2;
      }
  }

  Real Renderer3D::edgeFunction(Vector3 v0, Vector3 v1, Vector3 p)
  {
    Real val = (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
    return val;
  }


  //TODO remove and make a proper quaternion system
  Matrix3 Renderer3D::computeRotationMatrix(Vector3 angles)
  {
    Real cosAlpha = cosf(angles.x);
    Real sinAlpha = sinf(angles.x);
    Real cosBeta = cosf(angles.y);
    Real sinBeta = sinf(angles.y);
    Real cosGamma = cosf(angles.z);
    Real sinGamma = sinf(angles.z);

    Matrix3 A = {{{R(1.0), R(0.0) , R(0.0) },{R(0.0), cosAlpha, -sinAlpha},{R(0.0), sinAlpha, cosAlpha }}}; //X
    Matrix3 B = {{{cosBeta , R(0.0), sinBeta},{R(0.0) ,R(1.0), R(0.0)},{-sinBeta, R(0.0), cosBeta}}}; //Y
    Matrix3 C = {{{cosGamma, -sinGamma, R(0.0)},{sinGamma, cosGamma , R(0.0)},{R(0.0), R(0.0), R(1.0)}}}; //Z

    return A*B*C;
  }

}
