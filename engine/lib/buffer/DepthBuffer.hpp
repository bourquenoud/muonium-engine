/*
 * DepthBuffer.h
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#ifndef DEPTHBUFFER_H_
#define DEPTHBUFFER_H_

#include <cstdint>

#include "../texture.hpp"
#include "../arith.hpp"

namespace ue
{

  class DepthBuffer
  {
  public:
    uint16_t width;
    uint16_t height;
    Real* buffer;

    DepthBuffer();
    DepthBuffer(uint16_t width_, uint16_t height_, Real* buffer_);

    Real operator[](int);
  };

}

#endif /* DEPTHBUFFER_H_ */
