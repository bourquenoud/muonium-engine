/*
 * DepthBuffer.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "DepthBuffer.h"

namespace ue
{
  DepthBuffer::DepthBuffer()
   {

   }
  DepthBuffer::DepthBuffer(uint16_t width_, uint16_t height_, Real* buffer_)
   {
     width = width_;
     height = height_;
     buffer = buffer_;
   }

  Real DepthBuffer::operator[](int i)
   {
     return buffer[i];
   }
}
