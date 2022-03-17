/*
 * LightSun.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "LightSun.hpp"

namespace ue
{
  LightSun::LightSun()
  {
  }

  LightSun::LightSun(Vector3 v, Real i)
  {
    direction = v;
    intensity = i;
  }
}
