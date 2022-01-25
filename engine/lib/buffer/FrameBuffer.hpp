/*
 * FrameBuffer.h
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <cstdint>

#include "../texture.hpp"

namespace ue
{

  class FrameBuffer
  {
  public:
    uint16_t width;
    uint16_t height;
    Colour* buffer;

    FrameBuffer();
    FrameBuffer(uint16_t width_, uint16_t height_, Colour* buffer_);

    Colour& operator[](int i)
    {
      return buffer[i];
    }
  };

}

#endif /* FRAMEBUFFER_H_ */
