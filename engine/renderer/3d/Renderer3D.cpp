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
    //clearFrameBufferGrid((Colour){0xFFFFFFFF}, (Colour){0xFFCCCCCC}, 12);

    //Works object by object
    for(uint32_t i = 0; i < objectNumber; i++)
      {
        renderObject(objectList[i]);
      }

    //drawn the background
    drawBackgroundGrid((Colour){0xFFFFFFFF}, (Colour){0xFFCCCCCC}, 12);
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
        if(projTriangle.va->z > camera.far && projTriangle.vb->z > camera.far && projTriangle.vc->z > camera.far)
          continue;

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

        renderTriangle(projTriangle, minCorner, maxCorner, o);
      }
  }

  void Renderer3D::renderTriangle(Triangle& t,Vector2& minCorner,Vector2& maxCorner, Poly& object)
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


#if UE_CONFIG_ENABLE_NORMAL == false
    //Light depend of the normal, which everywhere on a flat triangle
    Real light = Real::min(computeLight(t), R(1.0));
#if UE_CONFIG_ENABLE_TEXTURE == false
    //If textures and normals are disabled we can process the colour in advance
    Colour col;
    col.colour.r = (uint8_t)((Real)0x7F * light);
    col.colour.g = (uint8_t)((Real)0x1F * light);
    col.colour.b = (uint8_t)((Real)0x80 * light);
    col.colour.a = 0xFF;
#endif
#endif

    //Starting point barycentric coordinates
    Real startW0 = k[0][0]*minCorner.x + k[0][1]*minCorner.y + k[0][2];
    Real startW1 = k[1][0]*minCorner.x + k[1][1]*minCorner.y + k[1][2];
    Real startW2 = k[2][0]*minCorner.x + k[2][1]*minCorner.y + k[2][2];

    //Precompute z for interpolation
    Real startZ = (startW0 * t.vc->z + startW1 * t.va->z + startW2 * t.vb->z) * area;
    Real zIncX = (k[0][0] * t.vc->z + k[1][0] * t.va->z + k[2][0] * t.vb->z) * area;
    Real zIncY = (k[0][1] * t.vc->z + k[1][1] * t.va->z + k[2][1] * t.vb->z) * area;

    /* NOTE: Doesn't work
    //Precompute texture position for interpolation
    Vector2 startTexPos;
    startTexPos.x = Real::clamp((startW0 * t.vtc->x + startW1 * t.vta->x + startW2 * t.vtb->x) * area, 0, 1);
    startTexPos.y = Real::clamp((startW0 * t.vtc->y + startW1 * t.vta->y + startW2 * t.vtb->y) * area, 0, 1);
    Real texPosXIncX = (k[0][0] * t.vtc->x + k[1][0] * t.vta->x + k[2][0] * t.vtb->x) * area;
    Real texPosXIncY = (k[0][1] * t.vtc->x + k[1][1] * t.vta->x + k[2][1] * t.vtb->x) * area;
    Real texPosYIncX = (k[0][0] * t.vtc->y + k[1][0] * t.vta->y + k[2][0] * t.vtb->y) * area;
    Real texPosYIncY = (k[0][1] * t.vtc->y + k[1][1] * t.vta->y + k[2][1] * t.vtb->y) * area;
    */

    //Casts
    uint32_t width = (uint32_t)camera.width;

    uint32_t i = 0;

    //TODO Optimise rendering order (Scanlines direction)
    for(uint16_t x = (uint32_t)minCorner.x; x <= (uint16_t)maxCorner.x; x++)
      {
        Real w0 = startW0;
        Real w1 = startW1;
        Real w2 = startW2;

        Real z = startZ;

        for(uint16_t y = (uint32_t)minCorner.y; y <= (uint16_t)maxCorner.y; y++)
          {

            //TODO: use a | sign bit computation instead
            if(w0 >= R(0) && w1 >= R(0) && w2 >= R(0))
              {
                /*
                Real z = w0 * t.vc->z + w1 * t.va->z + w2 * t.vb->z;
                z *= area;*/

                i = x + y * width;

                //Check the depth and draw if closer
                if(z > depthBuffer[i])
                  {
                    depthBuffer[i] = z;

                    //Sample the texture
#if UE_CONFIG_ENABLE_TEXTURE == true
                    Vector2 texPos;
                    texPos.x = Real::clamp(
                        (w0 * t.vtc->x + w1 * t.vta->x + w2 * t.vtb->x) * area,
                        0, 1);
                    texPos.y = Real::clamp(
                        (w0 * t.vtc->y + w1 * t.vta->y + w2 * t.vtb->y) * area,
                        0, 1);

                    Colour col = object.texture.getPixelAt(texPos);

                    col.colour.r = (uint8_t)((Real)col.colour.r * light);
                    col.colour.g = (uint8_t)((Real)col.colour.g * light);
                    col.colour.b = (uint8_t)((Real)col.colour.b * light);
#endif

                    frameBuffer[i] = col;
                  }
              }
            w0 += k[0][1];
            w1 += k[1][1];
            w2 += k[2][1];

            z += zIncY;
          }

        //Move on y
        startW0 += k[0][0];
        startW1 += k[1][0];
        startW2 += k[2][0];

        startZ += zIncX;
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

  void Renderer3D::drawBackgroundGrid(Colour col1, Colour col2, uint32_t size)
  {
    for(uint32_t i = 0; i < frameBuffer.width * frameBuffer.height; i++)
      {
        //XXX BAD
        if(depthBuffer[i] < R(-1000.0))
          {
            uint32_t x = i%frameBuffer.width;
            uint32_t y = i/frameBuffer.width;

            frameBuffer[i] = (((x/size) & 1)^((y/size) & 1))?col1:col2;
          }
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
    Real cosAlpha = cosf((float)angles.x);
    Real sinAlpha = sinf((float)angles.x);
    Real cosBeta = cosf((float)angles.y);
    Real sinBeta = sinf((float)angles.y);
    Real cosGamma = cosf((float)angles.z);
    Real sinGamma = sinf((float)angles.z);

    Matrix3 A = {{{R(1.0), R(0.0) , R(0.0) },{R(0.0), cosAlpha, -sinAlpha},{R(0.0), sinAlpha, cosAlpha }}}; //X
    Matrix3 B = {{{cosBeta , R(0.0), sinBeta},{R(0.0) ,R(1.0), R(0.0)},{-sinBeta, R(0.0), cosBeta}}}; //Y
    Matrix3 C = {{{cosGamma, -sinGamma, R(0.0)},{sinGamma, cosGamma , R(0.0)},{R(0.0), R(0.0), R(1.0)}}}; //Z

    return A*B*C;
  }

}
