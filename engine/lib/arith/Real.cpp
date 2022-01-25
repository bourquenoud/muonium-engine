/*
 * Real.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Real.hpp"

namespace ue
{
  //****Basic arithmetic****
  Real Real::min(Real a, Real b)
  {
    return (a<b)?a:b;
  }
  Real Real::min(Real a,Real b,Real c)
  {
    return min(a,min(b,c));
  }
  Real Real::max(Real a,Real b)
  {
    return (a>b)?a:b;
  }
  Real Real::max(Real a,Real b,Real c)
  {
    return max(a,max(b,c));
  }
  Real Real::clamp(Real val, Real minVal, Real maxVal)
  {
    return max(min(val, maxVal), minVal);
  }
  Real Real::abs(Real a)
  {
    return (a < R(0.0))?-a:a;
  }

}


