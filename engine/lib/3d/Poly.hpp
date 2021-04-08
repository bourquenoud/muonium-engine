/*
 * Poly.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef POLY2_H_
#define POLY2_H_

#include "../arith.hpp"
#include "../linal.hpp"
#include "../texture.hpp"
#include "../texture/Texture.hpp"

#include "Triangle.hpp"
#include "Object3D.hpp"

namespace ue {

  class Poly : public Object3D
  {
  public:
    Triangle* faces;
    uint32_t faceCount;

    Vector3* vertices;
    uint32_t vertexCount;

#if UE_CONFIG_ENABLE_TEXTURE == true
    Texture texture;
    Vector2* textureVerts;
    uint32_t textureCount;
#endif //UE_CONFIG_TEXTURE

#if UE_CONFIG_ENABLE_NORMAL == true
    Vector3* normals;
    uint32_t normalCount;
#endif //UE_CONFIG_NORMAL

    Poly();
    Poly(const Poly& object);
  };

}

#endif /* POLY_H_ */
