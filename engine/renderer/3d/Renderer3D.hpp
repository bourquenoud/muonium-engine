/*
 * Renderer3D.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef RENDERER3D_H_
#define RENDERER3D_H_

#include <cstdint>
#include <cstdio>
#include <cassert>

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
    Real ambientLight;
    Poly* objectList;
    uint32_t objectNumber;

    void RenderFullFrame(void);

  private:
    //XXX this is getting messy...
    void renderObject(Poly);
    void renderTriangle(Triangle);
    Real edgeFunction(Vector3&,Vector3&,Vector3&);
    void renderTriangle(Triangle&,Vector2&,Vector2&,Poly&);
    void renderSmallTriangle(Triangle&,Vector2&,Vector2&,Poly&,Real&);
    void clearDepthBuffer();
    void clearFrameBuffer(Colour);
    void clearFrameBufferGrid(Colour,Colour,uint32_t);
    void drawBackgroundGrid(Colour,Colour,uint32_t);
    Matrix3 computeRotationMatrix(Vector3 angles);
    Real computeLight(Triangle triangle);
  };

}

#endif /* RENDERER3D_H_ */
