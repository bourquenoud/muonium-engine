/*
 * Particle.hpp
 *
 *  Created on: 20 Apr 2021
 *      Author: mathieu
 */

#ifndef LIB_3D_PARTICLE_HPP_
#define LIB_3D_PARTICLE_HPP_

#include "../texture.hpp"
#include "../linal.hpp"
#include "../arith.hpp"

namespace ue
{

  class Particle
  {
  public:
    Texture texture;
    Vector2 size; //Size in world dimension
    Vector3 position;
    Real rotation; //TODO: make use of it
    Real transparency; //0 to 1

    //Use masks only if there is no alpha channel
    #if UE_CONFIG_COLOUR != ARGB8888 \
      and UE_CONFIG_COLOUR != RGBA8888 \
      and UE_CONFIG_COLOUR != ARGB1555 \
      and UE_CONFIG_COLOUR != RGBA5551
//        Mask mask;
    #endif
  };

} /* namespace ue */

#endif /* LIB_3D_PARTICLE_HPP_ */
