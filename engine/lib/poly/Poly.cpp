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
    center = object.center;
    faceCount = object.faceCount;
    faces = object.faces;
    texture = object.texture;
    textureVerts = object.textureVerts;
    textureCount = object.vertexCount;
    vertices = object.vertices;
  }
}
