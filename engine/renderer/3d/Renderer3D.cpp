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

        *(projTriangle.va) = camera.toRaster(*(o.faces[i].va));
        *(projTriangle.vb) = camera.toRaster(*(o.faces[i].vb));
        *(projTriangle.vc) = camera.toRaster(*(o.faces[i].vc));

        //Don't process triangles behind the camera clipping plane (z is reversed)
        if(projTriangle.va->z < R(0.0) && projTriangle.vb->z < R(0.0) && projTriangle.vc->z < R(0.0))
          continue;

        //Don't process triangles that are too far
        if(projTriangle.va->z > camera.far && projTriangle.vb->z > camera.far && projTriangle.vc->z > camera.far)
          continue;

        //Process the bounding rectangle
        Vector2 maxCorner = Vector2(
            Real::max(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x),
            Real::max(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y));

        Vector2 minCorner = Vector2(
            Real::min(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x),
            Real::min(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y));

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

    //Pre-compute factors for the triangle drawing
    Real k[3][3];
    k[0][0] = t.va->y - t.vb->y;
    k[0][1] = t.vb->x - t.va->x;
    k[0][2] = t.va->y*t.vb->x + t.vb->y*t.va->x;

    k[1][0] = t.vb->y - t.vc->y;
    k[1][1] = t.vc->x - t.vb->x;
    k[1][2] = t.vb->y*t.vc->x + t.vc->y*t.vb->x;

    k[2][0] = t.vc->y - t.va->y;
    k[2][1] = t.va->x - t.vc->x;
    k[2][2] = t.vc->y*t.va->x + t.va->y*t.vc->x;

    //Compute signed the area
    Real area = k[0][0]*t.vc->x + k[0][1]*t.vc->y + k[0][2];

    //If the area if negative the triangle is seen for the back
    // TODO: add the option to disable back culling
    if (area <= R(0))
      return;

    //Divide everything by the area (Precision loss with Fixed32 and big screen)
    area = R(1)/area;
    k[0][0] *= area;
    k[0][1] *= area;
    k[0][2] *= area;
    k[1][0] *= area;
    k[1][1] *= area;
    k[1][2] *= area;
    k[2][0] *= area;
    k[2][1] *= area;
    k[2][2] *= area;

    //Starting point barycentric coordinates
    Real w0 = k[0][0]*minCorner.x + k[0][1]*minCorner.y + k[0][2];
    Real w1 = k[1][0]*minCorner.x + k[1][1]*minCorner.y + k[1][2];
    Real w2 = k[2][0]*minCorner.x + k[2][1]*minCorner.y + k[2][2];

    //Scan in the x direction
    for(uint16_t y = (uint32_t)minCorner.y; y < (uint32_t)maxCorner.y; y++)
      {
        for(uint16_t x = (uint32_t)minCorner.x; x < (uint32_t)maxCorner.x; x++)
          {
            //TODO: use a | sign bit computation instead
            if(w0 > R(0) && w1 > R(0) && w2 > R(0))
              {
                Real z = w0 * t.vc->z + w1 * t.va->z + w2 * t.vb->z;
                uint32_t i = x+y*camera.width;
                //Check the depth and draw if closer
                if(z < depthBuffer[i])
                  {
                    depthBuffer[i] = z;
                    //XXX THIS IS A TEMPORARY TEST XXX
                    Colour col;
                    col.colour.r = 0x00;
                    col.colour.g = 0x00;
                    col.colour.b = 0x00;
                    col.colour.a = 0xFF;
                    frameBuffer[i] = col;
                    //XXX ************************ XXX
                  }
              }

            w0 += k[0][0];
            w1 += k[1][0];
            w2 += k[2][0];
          }
        w0 += k[0][1];
        w1 += k[1][1];
        w2 += k[2][1];
      }



  }

}
