/*
 * Renderer3D.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef RENDERER3D_H_
#define RENDERER3D_H_

#include <cstdint>

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
    Camera camera;
    LightSun sun;
    Poly* objectList;
    uint32_t objectNumber;

    void RenderFullFrame(void);

  private:
    void renderObject(Poly);
    void renderTriangle(Triangle);
    Real edgeFunction(Vector3,Vector3,Vector3)
  };

}

#endif /* RENDERER3D_H_ */
