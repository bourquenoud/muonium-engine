/*
 * FrameBuffer.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "FrameBuffer.h"

namespace ue
{
  FrameBuffer::FrameBuffer()
  {

  }
  FrameBuffer::FrameBuffer(uint16_t width_, uint16_t height_, Colour* buffer_)
  {
    width = width_;
    height = height_;
    buffer = buffer_;
  }

  Colour FrameBuffer::operator[](int i)
  {
    return buffer[i];
  }
}
