/*
 * LightSun.hpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#ifndef LIGHTSUN_HPP_
#define LIGHTSUN_HPP_

#include "../arith.hpp"
#include "../texture.hpp"
#include "../linal.hpp"

namespace ue
{

  class LightSun
  {
  public:
    Vector3 direction;
    Real intensity;
#if UE_CONFIG_ENABLE_COLORED_LIGHT == true
    Colour lightColour;
#endif
  };

}

#endif /* LIGHTSUN_HPP_ */
