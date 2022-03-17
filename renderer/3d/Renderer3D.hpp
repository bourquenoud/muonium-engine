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

#include "../../lib/3d.hpp"
#include "../../lib/buffer.hpp"
#include "../../lib/arith.hpp"
#include "../../lib/linal.hpp"
#include "../../lib/texture.hpp"

namespace ue
{

  class Renderer3D {
  public:
    FrameBuffer frameBuffer;
    DepthBuffer depthBuffer;
    Camera* camera;
    LightSun* sun;
    Real ambientLight;
    uint16_t renderSpanIndex;

    Renderer3D();
    Renderer3D(Renderer3D&);

    void renderObject(const Poly&);
    void renderParticle(Particle);

    void clearDepthBuffer();
    void clearFrameBuffer(Colour);
    void clearFrameBufferGrid(Colour,Colour,uint32_t);
    void clearFrameBufferSkybox(CubeMap skybox);
    void drawBackgroundGrid(Colour,Colour,uint32_t);

    void renderObject_old(const Poly&);

  private:


    //XXX this is getting messy...
    typedef enum eClipAxis
    {
      TOP = 0, // & 0x02 == 0 -> X axis
      BOTTOM = 1,
      LEFT = 2,
      RIGHT = 3
    } ClipAxis;

    typedef struct VerticesBuffer //XXX: bad naming
    {
        Vector3 v; //Vertex
#if UE_CONFIG_ENABLE_TEXTURE == true
        Vector2 vt; //Texture vertex
#endif
#if UE_CONFIG_ENABLE_SMOOTH_NORMAL == true
        Vector3 vn; //Normal vertex
#endif
    } VerticesBuffer;

    void renderTriangle(Triangle);
    Real edgeFunction(Vector3&,Vector3&,Vector3&);
    void renderTriangle(const Triangle&,const Vector2&,const Vector2&, const Poly&);
    void renderTriangle(const Triangle&,const Vector2&,const Vector2&,const Poly&, int, int);
    //void renderSmallTriangle(Triangle&,Vector2&,Vector2&,Poly&,Real&);
    Matrix3 computeRotationMatrix(Vector3 angles);
    Real computeLight(Triangle);
    uint16_t clipTriangle(int,Triangle*,int&,VerticesBuffer*,int&,Real,ClipAxis);
  };

}

#endif /* RENDERER3D_H_ */
