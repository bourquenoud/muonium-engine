/*
 * Texture.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef TEXTURE2_H_
#define TEXTURE2_H_

#include <cstdint>
#include <cstdio>

#include "Colour.hpp"
#include "../arith.hpp"
#include "../linal.hpp"

namespace ue {

  class Texture
  {
  public:
    uint32_t width;
    uint32_t height;
    Colour* pixel;

    Colour getPixelAt(Vector2&);
  };

}

#endif /* TEXTURE2_H_ */
