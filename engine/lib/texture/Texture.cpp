/*
 * Texture.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Texture.hpp"

namespace ue
{
  Colour Texture::getPixelAt(Vector2& pos)
  {
    uint32_t x = (uint32_t)Real::floor(pos.x * (Real)(width - 1));
    uint32_t y = (uint32_t)Real::floor((R(1.0) - pos.y) * (Real)(height - 1));

     return pixel[x + (width * y)];
  }
}
