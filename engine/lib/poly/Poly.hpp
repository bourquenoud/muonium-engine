/*
 * Poly.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef POLY_H_
#define POLY_H_

#include "../arith.hpp"
#include "../linal.hpp"
#include "../texture.hpp"
#include "../texture/Texture.hpp"

#include "Triangle.hpp"

namespace ue {

  class Poly
  {
  public:
    Vector3 center;
    Triangle* faces;
    uint32_t faceCount;

    Vector3* vertices;
    uint32_t vertexCount;

#ifdef UE_CONFIG_TEXTURE
    Texture texture;
    Vector2* textureVerts;
    uint32_t textureCount;
#endif //UE_CONFIG_TEXTURE

#ifdef UE_CONFIG_NORMAL
    Vector3* normals;
    uint32_t normalCount;
#endif //UE_CONFIG_NORMAL

    Poly();
    Poly(const Poly& object);
  };

}

#endif /* POLY_H_ */
