/*
 * Texture.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef TEXTURE2_H_
#define TEXTURE2_H_

#include <cstdint>

#include "Colour.hpp"

namespace ue {

  class Texture
  {
  public:
    uint32_t width;
    uint32_t height;
    Colour* pixel;
  };

}

#endif /* TEXTURE2_H_ */
