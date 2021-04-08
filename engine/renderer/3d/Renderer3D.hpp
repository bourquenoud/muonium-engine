/*
 * Renderer3D.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef RENDERER3D_H_
#define RENDERER3D_H_

#include "../../lib/arith.hpp"
#include "../../lib/buffer.hpp"
#include "../../lib/linal.hpp"
#include "../../lib/poly.hpp"
#include "../../lib/texture.hpp"

namespace ue
{

  class Renderer3D {
  public:
    FrameBuffer frameBuffer;
    DepthBuffer depthBuffer;
    Camera worldCamera;
    Poly* objectList;
  };

}

#endif /* RENDERER3D_H_ */
