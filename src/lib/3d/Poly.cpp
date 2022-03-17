/*
 * Poly.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Poly.hpp"

namespace ue
{
  Poly::Poly()
  {

  }

  Poly::Poly(const Poly& object)
  {
    rotation = object.rotation;
    position = object.position;
    faceCount = object.faceCount;
    faces = object.faces;
    vertices = object.vertices;

#if UE_CONFIG_ENABLE_TEXTURE == true
    texture = object.texture;
    textureVerts = object.textureVerts;
    textureCount = object.vertexCount;
#endif

#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
    normals = object.normals;
    normalCount = object.normalCount;
#endif
  }
}
