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

        //Convert to camera perspective and compute the texture vertices correction
        Triangle projTriangle;

        *(projTriangle.va) = camera.toRaster(*(o.faces[i].va));
        *(projTriangle.vb) = camera.toRaster(*(o.faces[i].vb));
        *(projTriangle.vc) = camera.toRaster(*(o.faces[i].vc));

        //Don't process triangles behind the camera clipping plane (z is reversed)
        if(projTriangle.va->z < R(0.0) && projTriangle.vb->z < R(0.0) && projTriangle.vc->z < R(0.0))
          continue;

        //Don't process triangles that are too far
        if(projTriangle.va->z > camera.far && projTriangle.vb->z > camera.far && projTriangle.vc->z > camera.far)
          continue;

        //Copy the texture vectors xxx Probably useless xxx
        *(projTriangle.vta) = *(triangle.vta);
        *(projTriangle.vtb) = *(triangle.vtb);
        *(projTriangle.vtc) = *(triangle.vtc);



      }
  }

  Real Renderer3D::edgeFunction(Vector3 v0, Vector3 v1, Vector3 p)
  {
          float val = (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
          return val;
  }

}
