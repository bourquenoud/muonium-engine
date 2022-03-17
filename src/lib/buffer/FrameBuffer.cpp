/*
 * FrameBuffer.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "FrameBuffer.hpp"

namespace ue
{
  FrameBuffer::FrameBuffer()
  {
    width = 0;
    height = 0;
    buffer = NULL;
  }
  FrameBuffer::FrameBuffer(uint16_t width_, uint16_t height_, Colour *buffer_)
  {
    width = width_;
    height = height_;
    buffer = buffer_;
  }

}
