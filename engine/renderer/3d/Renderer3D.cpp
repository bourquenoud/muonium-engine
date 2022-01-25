/*
 * Renderer3D.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Renderer3D.hpp"

//#define SMALL_TRIANGLE_THRESHOLD (R(1))

namespace ue
{
  Renderer3D::Renderer3D()
    {
    renderSpanIndex = 0;
    }
  Renderer3D::Renderer3D(Renderer3D& r)
  {
    frameBuffer = r.frameBuffer;
    depthBuffer = r.depthBuffer;
    camera = r.camera;
    sun = r.sun;
    ambientLight = r.ambientLight;
    renderSpanIndex = r.renderSpanIndex;
  }

  void Renderer3D::renderObject(const Poly& o)
  {
	  //TODO: put that in it's own function (once it works correctly)
      //Compute the transform matrix
      ue::Vector3 offset;
      ue::Matrix4 transformMatrix;
      ue::Quaternion camInverse = (*camera).rotation;

      offset = o.position - camInverse.rotationMatrix3() * (*camera).position;
      transformMatrix = (o.rotation*camInverse).rotationMatrix4(); //Compute the rotation matrix
      transformMatrix[0][3] = offset.x;
      transformMatrix[1][3] = offset.y;
      transformMatrix[2][3] = offset.z;

      //
  }

  __attribute__((deprecated("Use only for testing")))
  void Renderer3D::renderObject_old(const Poly& o)
  {
      //Compute the transform matrix
      ue::Vector3 offset;
      ue::Matrix4 transformMatrix;
      ue::Quaternion camInverse = (*camera).rotation;

      offset = o.position - camInverse.rotationMatrix3() * (*camera).position;
      transformMatrix = (o.rotation*camInverse).rotationMatrix4(); //Compute the rotation matrix
      transformMatrix[0][3] = offset.x;
      transformMatrix[1][3] = offset.y;
      transformMatrix[2][3] = offset.z;


#if UE_CONFIG_FAST_CLIPPING == false
    //TODO: make it cleaner and calculate max number of triangles/vertices
    //Temporary arrays for triangle clipping, avoids using heap
    Triangle triangleList[8]; //8 Is the maximum number of new triangle when clipping
    VerticesBuffer vertices[16]; //16 has not been calculated correctly. It is an overestimate
#endif //UE_CONFIG_FAST_CLIPPING

    for(uint32_t i = 0; i < o.faceCount; i++)
      {
        //TODO : make it cleaner
        //Convert to camera perspective and compute the texture vertices correction
        Vector3 projVertexA, projVertexB, projVertexC;
        Triangle projTriangle = { &projVertexA, &projVertexB, &projVertexC };

        *(projTriangle.va) =  transformMatrix * (*(o.faces[i].va));
        *(projTriangle.vb) =  transformMatrix * (*(o.faces[i].vb));
        *(projTriangle.vc) = transformMatrix * (*(o.faces[i].vc));

        //Discard if any is too close (aka FAST_CLIPPING XXX: defines not configured correctly)
        if(projTriangle.va->z < (*camera).near | projTriangle.vb->z < (*camera).near | projTriangle.vc->z < (*camera).near)
        	continue;

        //If not too close the continue projection
        *(projTriangle.va) =  (*camera).toRaster(*(projTriangle.va));
        *(projTriangle.vb) =  (*camera).toRaster(*(projTriangle.vb));
        *(projTriangle.vc) =  (*camera).toRaster(*(projTriangle.vc));

        //Don't process triangles behind the camera clipping plane (z is reversed)
        if(projTriangle.va->z > R(0.0) && projTriangle.vb->z > R(0.0) && projTriangle.vc->z > R(0.0))
          continue;

        //Don't process triangles that are too far
        if(projTriangle.va->z > (*camera).far && projTriangle.vb->z > (*camera).far && projTriangle.vc->z > (*camera).far)
          continue;

#if UE_CONFIG_ENABLE_TEXTURE == true
        //Copy the texture vectors pointers
        projTriangle.vta = o.faces[i].vta;
        projTriangle.vtb = o.faces[i].vtb;
        projTriangle.vtc = o.faces[i].vtc;
#endif

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        //Copy the normal vectors
        projTriangle.vna = o.faces[i].vna;
        projTriangle.vnb = o.faces[i].vnb;
        projTriangle.vnc = o.faces[i].vnc;
#endif

#if UE_CONFIG_FAST_CLIPPING == true

        //Process the bounding rectangle
        Vector2 maxCorner = Vector2(
            Real::round(Real::max(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x)),
            Real::round(Real::max(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y)));

        Vector2 minCorner = Vector2(
            Real::round(Real::min(projTriangle.va->x, projTriangle.vb->x, projTriangle.vc->x)),
            Real::round(Real::min(projTriangle.va->y, projTriangle.vb->y, projTriangle.vc->y)));

        //Skip the triangle if the bounding boxes doesn't overlap with the screen
        if(minCorner.x < camera.width && R(0.0) < maxCorner.x && minCorner.y < camera.height && R(0.0) < maxCorner.y)
          renderTriangle(projTriangle, minCorner, maxCorner, o);
        else
          continue; //Can be removed because it is the last instruction of the loop
#else
        //Load the triangle into the array as the first triangle
        int triangleListCount = 1;
        int vertexListCount = 0;
        int lastTriangleCount = 0;

        triangleList[0] = projTriangle;

        //Clip TOP (only one triangle)
        int result = clipTriangle(0, triangleList, triangleListCount, vertices,
            vertexListCount, (Real)((renderSpanIndex) * frameBuffer.height) - R(0.5), TOP); //Y is reversed in screen space (-0.4 because it makes weird artifacts on the limit line)

        //Skip triangles that won't be rendered
        if(result == 0)
          continue;

        //Clip BOTTOM (clip all triangles in the list)
        lastTriangleCount = triangleListCount; //Updated in the loop, so we save the value before
        for(int i = 0; i < lastTriangleCount; i++)
          {
            if(triangleList[i].va != NULL) //Skip discarded triangles
              result = clipTriangle(i, triangleList, triangleListCount, vertices, vertexListCount, (Real)((renderSpanIndex + 1) * frameBuffer.height), BOTTOM); //Y is reversed in screen space
          }

        //Clip LEFT (clip all triangles in the list)
        lastTriangleCount = triangleListCount; //Updated in the loop, so we save the value before
        for(int i = 0; i < lastTriangleCount; i++)
          {
            if(triangleList[i].va != NULL) //Skip discarded triangles
              result = clipTriangle(i, triangleList, triangleListCount, vertices, vertexListCount, R(-1.0), LEFT);
          }

        //Clip RIGHT (clip all triangles in the list)
        lastTriangleCount = triangleListCount; //Updated in the loop, so we save the value before
        for(int i = 0; i < lastTriangleCount; i++)
          {
            if(triangleList[i].va != NULL) //Skip discarded triangles
              result = clipTriangle(i, triangleList, triangleListCount, vertices, vertexListCount, (Real)(*camera).width, RIGHT);
          }

        //Render all triangles
        for(int i = 0; i < triangleListCount; i++)
          {
            if(triangleList[i].va == NULL) //Skip discarded triangles
              continue;

            //Process the bounding rectangle
            Vector2 maxCorner = Vector2(
                Real::round(Real::max(triangleList[i].va->x, triangleList[i].vb->x, triangleList[i].vc->x)),
                Real::round(Real::max(triangleList[i].va->y, triangleList[i].vb->y, triangleList[i].vc->y)));

            Vector2 minCorner = Vector2(
                Real::round(Real::min(triangleList[i].va->x, triangleList[i].vb->x, triangleList[i].vc->x)),
                Real::round(Real::min(triangleList[i].va->y, triangleList[i].vb->y, triangleList[i].vc->y)));

#if (UE_CONFIG_FULL_FRAME_RENDER == true)
            renderTriangle(triangleList[i], minCorner, maxCorner, o);
#else
            renderTriangle(triangleList[i], minCorner, maxCorner, o, frameBuffer.height, frameBuffer.height * renderSpanIndex);
#endif
          }

#endif
      }
  }

  //FIXME: not working with FIXED32
  uint16_t Renderer3D::clipTriangle
  (int triangleIndex,
      Triangle* triangleList, int& triangleListCount,
      VerticesBuffer* vertexList, int& vertexListCount,
      Real position, ClipAxis axis)
  {
    //Count the number of vertices outside of the line. 3 -> fully hidden, 0 -> no intersection, 1 -> intersects and create 2 triangles, 2 -> intersects and create a single triangle
    int vertexOutsideFlags = 0;

    //Traded space for speed efficiency
    if(axis == BOTTOM)
      {
        vertexOutsideFlags |= (triangleList[triangleIndex].va->y > position); //Set a flag for which vertex is outside
        vertexOutsideFlags |= (triangleList[triangleIndex].vb->y > position)<<1;
        vertexOutsideFlags |= (triangleList[triangleIndex].vc->y > position)<<2;
      }
    else if(axis == TOP)
      {
        vertexOutsideFlags |= (triangleList[triangleIndex].va->y < position); //Set a flag for which vertex is outside
        vertexOutsideFlags |= (triangleList[triangleIndex].vb->y < position)<<1;
        vertexOutsideFlags |= (triangleList[triangleIndex].vc->y < position)<<2;
      }
    else if(axis == LEFT)
      {
        vertexOutsideFlags |= (triangleList[triangleIndex].va->x < position); //Set a flag for which vertex is outside
        vertexOutsideFlags |= (triangleList[triangleIndex].vb->x < position)<<1;
        vertexOutsideFlags |= (triangleList[triangleIndex].vc->x < position)<<2;
      }
    else //if(axis == RIGHT)
      {
        vertexOutsideFlags |= (triangleList[triangleIndex].va->x > position); //Set a flag for which vertex is outside
        vertexOutsideFlags |= (triangleList[triangleIndex].vb->x > position)<<1;
        vertexOutsideFlags |= (triangleList[triangleIndex].vc->x > position)<<2;
      }

    //XXX: is a switch really better than an if else chain ?
    //TODO: shrink the switch case. We can combine a lot of code
    switch(vertexOutsideFlags)
    {
    case 0b000: //Nothing to clip, keep the triangle in the triangle list
      return 1;
      //------------------------

    case 0b111: //Discard the triangle (set the vertices to NULL)
      triangleList[triangleIndex].va = NULL;
      //triangleList[triangleIndex].vb = NULL; //Not necessary
      //triangleList[triangleIndex].vc = NULL;
      return 0;
      //------------------------

      //NOTE: we can spend more time on the clipped triangles as they are much rarer than non clipped ones
      //One vertex outside
    case 0b001: //Clipping and will push two triangles to the triangle list
    case 0b010:
    case 0b100:
      {

        //Add the two new vertices and one triangle (they are already initialised in the array)
        vertexListCount+=2;
        triangleListCount++;

        VerticesBuffer vD,vE,vF; //vD is always outside

        if(vertexOutsideFlags == 0b001) //vA is outside
          {
            vD.v = *triangleList[triangleIndex].va;
            vE.v = *triangleList[triangleIndex].vb;
            vF.v = *triangleList[triangleIndex].vc;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vta;
            vE.vt = *triangleList[triangleIndex].vtb;
            vF.vt = *triangleList[triangleIndex].vtc;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vna;
            vE.vn = *triangleList[triangleIndex].vnb;
            vF.vn = *triangleList[triangleIndex].vnc;

#endif

          }
        if(vertexOutsideFlags == 0b010)
          {
            vD.v = *triangleList[triangleIndex].vb;
            vE.v = *triangleList[triangleIndex].va;
            vF.v = *triangleList[triangleIndex].vc;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vtb;
            vE.vt = *triangleList[triangleIndex].vta;
            vF.vt = *triangleList[triangleIndex].vtc;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vnb;
            vE.vn = *triangleList[triangleIndex].vna;
            vF.vn = *triangleList[triangleIndex].vnc;
#endif

          }
        if(vertexOutsideFlags == 0b100)
          {
            vD.v = *triangleList[triangleIndex].vc;
            vE.v = *triangleList[triangleIndex].va;
            vF.v = *triangleList[triangleIndex].vb;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vtc;
            vE.vt = *triangleList[triangleIndex].vta;
            vF.vt = *triangleList[triangleIndex].vtb;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vnc;
            vE.vn = *triangleList[triangleIndex].vna;
            vF.vn = *triangleList[triangleIndex].vnb;
#endif

          }

        //Interpolation factor
        Real t;

        //Compute interpolation factor
        t = (axis&0x02) ? ((position - vD.v.x) / (vE.v.x - vD.v.x)) : ((position - vD.v.y) / (vE.v.y - vD.v.y));
        //XXX: Check for overflows with FIXED32
        vertexList[vertexListCount-1].v = (vE.v * t) + (vD.v * (R(1.0) - t)); //Get the vertex position
#if UE_CONFIG_ENABLE_TEXTURE == true
        vertexList[vertexListCount-1].vt = (vE.vt * t) + (vD.vt * (R(1.0) - t));
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        vertexList[vertexListCount-1].vn = (vE.vn * t) + (vD.vn * (R(1.0) - t));
#endif


        //Compute interpolation factor
        t = (axis&0x02) ? ((position - vD.v.x) / (vF.v.x - vD.v.x)) : ((position - vD.v.y) / (vF.v.y - vD.v.y));
        //XXX: Check for overflows with FIXED32
        vertexList[vertexListCount-2].v = (vF.v * t) + (vD.v * (R(1.0) - t)); //Get the vertex position
#if UE_CONFIG_ENABLE_TEXTURE == true
        vertexList[vertexListCount-2].vt = (vF.vt * t) + (vD.vt * (R(1.0) - t));
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        vertexList[vertexListCount-2].vn = (vF.vn * t) + (vD.vn * (R(1.0) - t));
#endif


        //Edit vertices
        if(vertexOutsideFlags == 0b001) //A outside
          {
            triangleList[triangleIndex].va = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].va = &(vertexList[vertexListCount-2].v);
            triangleList[triangleListCount-1].vb = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].vc = triangleList[triangleIndex].vc;
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vta = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vta = &(vertexList[vertexListCount-2].vt);
            triangleList[triangleListCount-1].vtb = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vtc = triangleList[triangleIndex].vtc;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vna = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vna = &(vertexList[vertexListCount-2].vn);
            triangleList[triangleListCount-1].vnb = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vnc = triangleList[triangleIndex].vnc;
#endif

          }
        if(vertexOutsideFlags == 0b010) //B outside
          {
            triangleList[triangleIndex].vb = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].va = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].vb = &(vertexList[vertexListCount-2].v);
            triangleList[triangleListCount-1].vc = triangleList[triangleIndex].vc;
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vtb = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vta = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vtb = &(vertexList[vertexListCount-2].vt);
            triangleList[triangleListCount-1].vtc = triangleList[triangleIndex].vtc;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vnb = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vna = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vnb = &(vertexList[vertexListCount-2].vn);
            triangleList[triangleListCount-1].vnc = triangleList[triangleIndex].vnc;
#endif

          }
        if(vertexOutsideFlags == 0b100) //C outside
          {
            triangleList[triangleIndex].vc = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].va = &(vertexList[vertexListCount-2].v);
            triangleList[triangleListCount-1].vb = &(vertexList[vertexListCount-1].v);
            triangleList[triangleListCount-1].vc = triangleList[triangleIndex].vb;
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vtc = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vta = &(vertexList[vertexListCount-2].vt);
            triangleList[triangleListCount-1].vtb = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleListCount-1].vtc = triangleList[triangleIndex].vtb;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vnc = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vna = &(vertexList[vertexListCount-2].vn);
            triangleList[triangleListCount-1].vnb = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleListCount-1].vnc = triangleList[triangleIndex].vnb;
#endif

          }


        return 2; //TODO: Implement (should return 2)
        break;
        //------------------------
      }
      //Two vertices outside
    case 0b011: //Clipping and will push one triangle to the triangle list
    case 0b110:
    case 0b101:
      {
        //Add the two new vertices
        vertexListCount+=2;

        VerticesBuffer vD,vE,vF; //vD is always inside

        if(vertexOutsideFlags == 0b011) //vC is inside
          {
            vD.v = *triangleList[triangleIndex].vc;
            vE.v = *triangleList[triangleIndex].va;
            vF.v = *triangleList[triangleIndex].vb;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vtc;
            vE.vt = *triangleList[triangleIndex].vta;
            vF.vt = *triangleList[triangleIndex].vtb;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vnc;
            vE.vn = *triangleList[triangleIndex].vna;
            vF.vn = *triangleList[triangleIndex].vnb;
#endif

          }
        if(vertexOutsideFlags == 0b110)
          {
            vD.v = *triangleList[triangleIndex].va;
            vE.v = *triangleList[triangleIndex].vc;
            vF.v = *triangleList[triangleIndex].vb;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vta;
            vE.vt = *triangleList[triangleIndex].vtc;
            vF.vt = *triangleList[triangleIndex].vtb;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vna;
            vE.vn = *triangleList[triangleIndex].vnc;
            vF.vn = *triangleList[triangleIndex].vnb;
#endif

          }
        if(vertexOutsideFlags == 0b101)
          {
            vD.v = *triangleList[triangleIndex].vb;
            vE.v = *triangleList[triangleIndex].vc;
            vF.v = *triangleList[triangleIndex].va;
#if UE_CONFIG_ENABLE_TEXTURE == true
            vD.vt = *triangleList[triangleIndex].vtb;
            vE.vt = *triangleList[triangleIndex].vtc;
            vF.vt = *triangleList[triangleIndex].vta;
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            vD.vn = *triangleList[triangleIndex].vnb;
            vE.vn = *triangleList[triangleIndex].vnc;
            vF.vn = *triangleList[triangleIndex].vna;
#endif

          }

        //Interpolation factor
        Real t;
//FINISH THIS SHIT (texture interpolation)
        //Compute interpolation factor
        t = (axis&0x02) ? ((position - vD.v.x) / (vE.v.x - vD.v.x)) : ((position - vD.v.y) / (vE.v.y - vD.v.y));
        //XXX: Check for overflows with FIXED32
        vertexList[vertexListCount-1].v = (vE.v * t) + (vD.v * (R(1.0) - t)); //Get the vertex position
#if UE_CONFIG_ENABLE_TEXTURE == true
        vertexList[vertexListCount-1].vt = (vE.vt * t) + (vD.vt * (R(1.0) - t)); //Get the vertex position
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        vertexList[vertexListCount-1].vn = (vE.vn * t) + (vD.vn * (R(1.0) - t));
#endif


        //Compute interpolation factor
        t = (axis&0x02) ? ((position - vD.v.x) / (vF.v.x - vD.v.x)) : ((position - vD.v.y) / (vF.v.y - vD.v.y));
        //XXX: Check for overflows with FIXED32
        vertexList[vertexListCount-2].v = (vF.v * t) + (vD.v * (R(1.0) - t)); //Get the vertex position
#if UE_CONFIG_ENABLE_TEXTURE == true
        vertexList[vertexListCount-2].vt = (vF.vt * t) + (vD.vt * (R(1.0) - t)); //Get the vertex position
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        vertexList[vertexListCount-2].vn = (vF.vn * t) + (vD.vn * (R(1.0) - t));
#endif


        //Edit vertices
        if(vertexOutsideFlags == 0b011)
          {
            triangleList[triangleIndex].va = &(vertexList[vertexListCount-1].v);
            triangleList[triangleIndex].vb = &(vertexList[vertexListCount-2].v);
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vta = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleIndex].vtb = &(vertexList[vertexListCount-2].vt);
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vna = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleIndex].vnb = &(vertexList[vertexListCount-2].vn);
#endif

          }
        if(vertexOutsideFlags == 0b110)
          {
            triangleList[triangleIndex].vc = &(vertexList[vertexListCount-1].v);
            triangleList[triangleIndex].vb = &(vertexList[vertexListCount-2].v);
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vtc = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleIndex].vtb = &(vertexList[vertexListCount-2].vt);
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vnc = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleIndex].vnb = &(vertexList[vertexListCount-2].vn);
#endif

          }
        if(vertexOutsideFlags == 0b101)
          {
            triangleList[triangleIndex].vc = &(vertexList[vertexListCount-1].v);
            triangleList[triangleIndex].va = &(vertexList[vertexListCount-2].v);
#if UE_CONFIG_ENABLE_TEXTURE == true
            triangleList[triangleIndex].vtc = &(vertexList[vertexListCount-1].vt);
            triangleList[triangleIndex].vta = &(vertexList[vertexListCount-2].vt);
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
            triangleList[triangleIndex].vnc = &(vertexList[vertexListCount-1].vn);
            triangleList[triangleIndex].vna = &(vertexList[vertexListCount-2].vn);
#endif

          }
        return 1;
        break;
      }
      //------------------------
    }


    //Fully hidden -> Discard

    //no intersection -> Continue intersection checks

    //Intersects ->

    return 0;
  }

  //XXX This is the slowest thing I have ever seen, fix that
  void Renderer3D::renderParticle(Particle o)
  {
	  //TODO: make it work with colour types with no alpha
	  #if UE_CONFIG_COLOUR == ARGB8888 \
	   or UE_CONFIG_COLOUR == RGBA8888 \
	   or UE_CONFIG_COLOUR == ARGB1555 \
	   or UE_CONFIG_COLOUR == RGBA5551

    Vector3 startCorner = o.position;
    Vector3 endCorner = o.position;

    startCorner.x += o.size.x / R(2.0);
    startCorner.y += o.size.y / R(2.0);

    endCorner.x -= o.size.x / R(2.0);
    endCorner.y -= o.size.y / R(2.0);

    startCorner = (*camera).toRaster(startCorner);
    endCorner = (*camera).toRaster(endCorner);

    int startX = Real::max(startCorner.x, R(0.0));
    int startY = Real::max(startCorner.y, R(0.0));
    int endX = Real::min(endCorner.x, (*camera).width);
    int endY = Real::min(endCorner.y, (*camera).height);

    //Cast before, increases speed
    uint32_t width = (uint32_t)(*camera).width;

    //Interpolation step size (one step is one screen pixel)
    Real stepX = R(1.0)/(endCorner.x - startCorner.x); // (endCorner.x - startCorner.x) is the raterised size
    Real stepY = R(1.0)/(endCorner.y - startCorner.y);

    //Start position for the texture
    Vector2 startTexPos(
        (startCorner.x < R(0.0)) ? (Real::abs(startCorner.x) * stepX) : R(0.0),
            (startCorner.y < R(0.0)) ? (Real::abs(startCorner.y) * stepY) : R(0.0)
    );

    //Start at the starting coordinate
    Vector2 texPos(startTexPos);

    //TODO: render along the X axis
    for(int x = startX; x < endX; x++)
      {
        //Reset the starting coordinate
        texPos.y = startTexPos.y;
        for(int y = startY; y < endY; y++)
          {
            int i = x + y * width;
            if(startCorner.z > depthBuffer[i])
              {
                //Blend the texture
                Colour col = o.texture.getPixelAt(texPos);
                frameBuffer[i] = col.alphaBlendPre(frameBuffer[i]);
                //frameBuffer[i] = col.alphaClip(frameBuffer[i], o.transparency);
                //frameBuffer[i] = col.alphaClip(frameBuffer[i], R(0.5));
                //depthBuffer[i] = startCorner.z;
              }

            //Interpolation
            texPos.y += stepY;

            //Error accumulation can lead to a coordinate greater than 1 for the last pixels
            if(texPos.y >= R(1.0))
              break;
          }

        //Interpolation
        texPos.x += stepX;

        //Error accmulation can lead to a coordinate greater than 1 for the last pixels
        if(texPos.x >= R(1.0))
          break;
      }
#endif
  }

  void Renderer3D::renderTriangle(const Triangle& t, const Vector2& minCorner, const Vector2& maxCorner, const Poly& object)
  {
    //Compute the signed area
    Real area = edgeFunction(*(t.va), *(t.vb), *(t.vc));

    //If the area if negative the triangle is seen for the back
    // TODO: add the option to disable back culling
    if (area <= R(0.0))
      return;
    //TODO: Optimise for small triangles
    /*else if(area <= SMALL_TRIANGLE_THRESHOLD)
      {
        renderSmallTriangle(t, minCorner, maxCorner, object, area);
        return;
      }*/

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == false
    //Light depend of the normal, which is the same everywhere on a flat triangle
    Real light = Real::min(computeLight(t), R(1.0));
#if UE_CONFIG_ENABLE_TEXTURE == false
    //If textures and normals are disabled we can process the colour in advance TODO: Constant colour ? Boo ! You suck !
    Colour col;
    col.colour.r = (uint8_t)((Real)0x1F * light);
    col.colour.g = (uint8_t)((Real)0x3F * light);
    col.colour.b = (uint8_t)((Real)0x1F * light);
    //col.colour.a = 0xFF;
#endif
#endif


    //Precompute the area inverse for faster division
    area = R(1.0)/area;

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

    //Starting point barycentric coordinates
    Real startW0 = k[0][0]*minCorner.x + k[0][1]*minCorner.y + k[0][2];
    Real startW1 = k[1][0]*minCorner.x + k[1][1]*minCorner.y + k[1][2];
    Real startW2 = k[2][0]*minCorner.x + k[2][1]*minCorner.y + k[2][2];

    //Precompute z for interpolation
    Real startZ = (startW0 * t.vc->z + startW1 * t.va->z + startW2 * t.vb->z) * area;
    Real zIncX = (k[0][0] * t.vc->z + k[1][0] * t.va->z + k[2][0] * t.vb->z) * area;
    Real zIncY = (k[0][1] * t.vc->z + k[1][1] * t.va->z + k[2][1] * t.vb->z) * area;

    /* TODO: Make this work
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
    uint16_t width = (uint16_t)(*camera).width;
    uint16_t height = (uint16_t)(*camera).height;

    uint32_t i = 0;

    int16_t x;
    int16_t minCornerX = (int16_t)minCorner.x;
    int16_t minCornerY = (int16_t)minCorner.y;
    int16_t maxCornerX = (int16_t)maxCorner.x;
    int16_t maxCornerY = (int16_t)maxCorner.y;

    //Search for the first pixel to draw
    //TODO: can be replaced with a simple multiplication
    for(x = minCornerX; x <= maxCornerX; x++)
      {
        if(x >= 0) //Find the first pixel to draw
          break;

        //Move on y
        startW0 += k[0][0];
        startW1 += k[1][0];
        startW2 += k[2][0];

        startZ += zIncX;
      }

    //TODO: render along the X axis
    //Render triangle
    for(; x <= maxCornerX && x < width; x++)
      {
        Real w0 = startW0;
        Real w1 = startW1;
        Real w2 = startW2;

        Real z = startZ;

        int16_t y;

        //Search for the first pixel to draw
        for(y = minCornerY; y <= maxCornerY; y++)
          {
            if(w0 >= R(0.0) && w1 >= R(0.0) && w2 >= R(0.0) && y >= 0) //Find the first pixel to draw
              break;

            w0 += k[0][1];
            w1 += k[1][1];
            w2 += k[2][1];

            z += zIncY;
          }
        for(; y <= maxCornerY && y < height; y++)
          {

            //TODO: use a | sign bit computation instead
            if(w0 >= R(0.0) && w1 >= R(0.0) && w2 >= R(0.0))
              {
                /*
                Real z = w0 * t.vc->z + w1 * t.va->z + w2 * t.vb->z;
                z *= area;*/

                i = x + y * width;

                //Check the depth and draw if closer
                if(z > depthBuffer[i])
                  {
                    depthBuffer[i] = z;

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
                    Real light;
                    Vector3 normal = (*(t.vnc) * w0 + *(t.vna) * w1 + *(t.vnb) * w2);
                    normal = normal.normalise();

                    //Calculate the dot product
                    light = Real::max(R(0.0),(-(*sun).direction)*normal); //Prevent negative light

                    //Compute the directional light
                    light *= (*sun).intensity;

                    //Add ambient light
                    light += ambientLight;

                    //Limit to light value of 1
                    light = Real::min(light, R(1.0));
#if UE_CONFIG_ENABLE_TEXTURE == false
                    Colour col;
                    col.colour.r = (uint8_t)((Real)0x7F * light);
                    col.colour.g = (uint8_t)((Real)0x1F * light);
                    col.colour.b = (uint8_t)((Real)0x80 * light);
                    col.colour.a = 0xFF;
#endif //UE_CONFIG_ENABLE_TEXTURE
#endif //UE_CONFIG_ENABLE_SMOOTH_NORMAL

                    //Sample the texture
#if UE_CONFIG_ENABLE_TEXTURE == true
                    Vector2 texPos = (*(t.vtc) * w0 + *(t.vta) * w1 + *(t.vtb) * w2) * area;
                    //TODO: make it so we don't have to clamp the value
                    texPos.x = Real::clamp(texPos.x, 0, 1);
                    texPos.y = Real::clamp(texPos.y, 0, 1);

                    Colour col = object.texture.getPixelAt(texPos);

                    col.colour.r = (uint8_t)((Real)col.colour.r * light);
                    col.colour.g = (uint8_t)((Real)col.colour.g * light);
                    col.colour.b = (uint8_t)((Real)col.colour.b * light);
#endif//UE_CONFIG_ENABLE_TEXTURE

                    frameBuffer[i] = col;
                  }
              }
            else
              {
                //Stop when exiting the triangle
                //Reduce by 1/4 the number of pixel tested
                break;
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

  void Renderer3D::renderTriangle(const Triangle& t, const Vector2& minCorner,const Vector2& maxCorner, const Poly& object, int height, int height_offset)
  {
    //Compute the signed area
    Real area = edgeFunction(*(t.va), *(t.vb), *(t.vc));

    //If the area if negative the triangle is seen for the back
    // TODO: add the option to disable back culling
    if (area <= R(0.0))
      return;
    //TODO: Optimise for small triangles
    /*else if(area <= SMALL_TRIANGLE_THRESHOLD)
      {
        renderSmallTriangle(t, minCorner, maxCorner, object, area);
        return;
      }*/

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == false
    //Light depend of the normal, which is the same everywhere on a flat triangle
    Real light = Real::min(computeLight(t), R(1.0));
#if UE_CONFIG_ENABLE_TEXTURE == false
    //If textures and normals are disabled we can process the colour in advance TODO: Constant colour ? Boo ! You suck !
    Colour col;
    col.colour.r = (uint8_t)((Real)0x1F * light);
    col.colour.g = (uint8_t)((Real)0x3F * light);
    col.colour.b = (uint8_t)((Real)0x1F * light);
    //col.colour.a = 0xFF;
#endif
#endif


    //Precompute the area inverse for faster division
    area = R(1.0)/area;

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

    //Starting point barycentric coordinates
    Real startW0 = k[0][0]*minCorner.x + k[0][1]*minCorner.y + k[0][2];
    Real startW1 = k[1][0]*minCorner.x + k[1][1]*minCorner.y + k[1][2];
    Real startW2 = k[2][0]*minCorner.x + k[2][1]*minCorner.y + k[2][2];

    //Precompute z for interpolation
    Real startZ = (startW0 * t.vc->z + startW1 * t.va->z + startW2 * t.vb->z) * area;
    Real zIncX = (k[0][0] * t.vc->z + k[1][0] * t.va->z + k[2][0] * t.vb->z) * area;
    Real zIncY = (k[0][1] * t.vc->z + k[1][1] * t.va->z + k[2][1] * t.vb->z) * area;

    /* TODO: Make this work
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
    uint16_t width = (uint16_t)(*camera).width;

    uint32_t i = 0;

    int16_t x;
    int16_t minCornerX = (int16_t)minCorner.x;
    int16_t minCornerY = (int16_t)minCorner.y;
    int16_t maxCornerX = (int16_t)maxCorner.x;
    int16_t maxCornerY = (int16_t)maxCorner.y;

    //Search for the first pixel to draw
    //TODO: can be replaced with a simple multiplication
    for(x = minCornerX; x <= maxCornerX; x++)
      {
        if(x >= 0) //Find the first pixel to draw
          break;

        //Move on y
        startW0 += k[0][0];
        startW1 += k[1][0];
        startW2 += k[2][0];

        startZ += zIncX;
      }

    //TODO: render along the X axis
    //Render triangle
    for(; x <= maxCornerX && x < width; x++)
      {
        Real w0 = startW0;
        Real w1 = startW1;
        Real w2 = startW2;

        Real z = startZ;

        int16_t y;

        //Search for the first pixel to draw
        for(y = minCornerY; y <= maxCornerY; y++)
          {
            if(w0 >= R(0.0) && w1 >= R(0.0) && w2 >= R(0.0) && y >= 0) //Find the first pixel to draw
              break;

            w0 += k[0][1];
            w1 += k[1][1];
            w2 += k[2][1];

            z += zIncY;
          }
        for(; y <= maxCornerY && y < height + height_offset; y++)
          {

            //TODO: use a | sign bit computation instead
            if(w0 >= R(0.0) && w1 >= R(0.0) && w2 >= R(0.0))
              {
                /*
                Real z = w0 * t.vc->z + w1 * t.va->z + w2 * t.vb->z;
                z *= area;*/

                i = x + (y - height_offset) * width;

                //Check the depth and draw if closer
                if(z > depthBuffer[i])
                  {
                    depthBuffer[i] = z;

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
                    Real light;
                    Vector3 normal = (*(t.vnc) * w0 + *(t.vna) * w1 + *(t.vnb) * w2);
                    normal = normal.normalise();

                    //Calculate the dot product
                    light = Real::max(R(0.0),(-(*sun).direction)*normal); //Prevent negative light

                    //Compute the directional light
                    light *= (*sun).intensity;

                    //Add ambient light
                    light += ambientLight;

                    //Limit to light value of 1
                    light = Real::min(light, R(1.0));
#if UE_CONFIG_ENABLE_TEXTURE == false
                    Colour col;
                    col.colour.r = (uint8_t)((Real)0x7F * light);
                    col.colour.g = (uint8_t)((Real)0x1F * light);
                    col.colour.b = (uint8_t)((Real)0x80 * light);
                    col.colour.a = 0xFF;
#endif //UE_CONFIG_ENABLE_TEXTURE
#endif //UE_CONFIG_ENABLE_SMOOTH_NORMAL

                    //Sample the texture
#if UE_CONFIG_ENABLE_TEXTURE == true
                    Vector2 texPos = (*(t.vtc) * w0 + *(t.vta) * w1 + *(t.vtb) * w2) * area;
                    //TODO: make it so we don't have to clamp the value
                    texPos.x = Real::clamp(texPos.x, 0, 1);
                    texPos.y = Real::clamp(texPos.y, 0, 1);

                    Colour col = object.texture.getPixelAt(texPos);

                    col.colour.r = (uint8_t)((Real)col.colour.r * light);
                    col.colour.g = (uint8_t)((Real)col.colour.g * light);
                    col.colour.b = (uint8_t)((Real)col.colour.b * light);
#endif//UE_CONFIG_ENABLE_TEXTURE

                    frameBuffer[i] = col;
                  }
              }
            else
              {
                //Stop when exiting the triangle
                //Reduce by 1/4 the number of pixel tested
                break;
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
    //Calculate the normal
#if UE_CONFIG_ARITHMETIC == FIXED32
    //Normalise the vectors before (Will overflow in Fixed32 otherwise)
    s1 = a.approxNormalise().cross(b.approxNormalise());
#else
    s1 = a.cross(b);
#endif

    //Normalise the vectors
    s1 = s1.normalise();

    //Calculate the dot product
    light = Real::max(R(0.0),(-(*sun).direction)*s1); //Max prevent negative light

    //Compute the directional light
    light *= (*sun).intensity;

    //Add ambient light
    light += ambientLight;

    return light;
  }

  void Renderer3D::clearDepthBuffer()
  {
	  depthBuffer.clear();
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
        //TODO: remove the use of modulo and divide to improve performances
        uint32_t x = i%frameBuffer.width;
        uint32_t y = i/frameBuffer.width;

        frameBuffer[i] = (((x/size) & 1)^((y/size) & 1))?col1:col2;
      }
  }

  void Renderer3D::clearFrameBufferSkybox(CubeMap skybox)
  {
	  Matrix3 rotMat = (*camera).rotation.rotationMatrix3();

    for(uint32_t i = 0; i < frameBuffer.width * frameBuffer.height; i++)
      {
        //TODO: remove the use of modulo and divide to improve performances
        uint32_t x = i%frameBuffer.width;
        uint32_t y = i/frameBuffer.width + frameBuffer.height * renderSpanIndex;

        //Compute the world coordinates
        Vector3 coordinate = (*camera).fromRaster((Real)x, (Real)y);
        coordinate = rotMat * coordinate;

        //Sample the pixel
        frameBuffer[i] = skybox.getPixelAt(coordinate);
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

  Real Renderer3D::edgeFunction(Vector3& v0,Vector3& v1,Vector3& p)
  {
    Real val = (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
    return val;
  }


  //TODO: remove and make a proper quaternion system
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
