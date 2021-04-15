/*
 * Real.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Real.hpp"

namespace ue
{
  /****Constructor****///USING FLOATING POINT OPERATIONS
#if UE_CONFIG_ARITHMETIC == FLOAT
  Real::Real()
  {

  }
  Real::Real(int a)
  {
    val = (float)a;
  }
  Real::Real(unsigned int a)
  {
    val = (float)a;
  }
  Real::Real(float a)
  {
    val = a;
  }
  //USING 32bits FIXED POINT OPERATIONS
#elif UE_CONFIG_ARITHMETIC == FIXED32
  Real::Real()
  {

  }
  Real::Real(int a)
  {
    val = (Fixed32)a;
  }
  Real::Real(unsigned int a)
  {
    val = (Fixed32)a;
  }
  Real::Real(float a)
  {
    val = (Fixed32)a;
  }
#endif //UE_CONFIG_ARITHMETIC

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
    return (a < R(0))?-a:a;
  }

#if UE_CONFIG_ARITHMETIC == FLOAT
  Real Real::floor(Real a)
  {
    Real c;
    c.val = floorf((float)a.val);
    return c;
  }
  Real Real::ceil(Real a)
  {
    Real c;
    c.val = ceilf((float)a.val);
    return c;
  }
  Real Real::round(Real a)
  {
    Real c;
    c.val = roundf((float)a.val);
    return c;
  }
#elif UE_CONFIG_ARITHMETIC == FIXED32
  Real Real::floor(Real a)
  {
    Real c;
    //Mask the fractional part
    c.val.val = a.val.val & 0xFFFF0000;
    return c;
  }
  Real Real::ceil(Real a)
  {
    Real c;
    //+1 then floor except for round numbers
    c.val.val = ((a.val.val + 0x0000FFFF) & 0xFFFF0000);
    return c;
  }
  Real Real::round(Real a)
  {
    Real c;
    //+0.5 then floor
    c.val.val = ((a.val.val + 0x00008000) & 0xFFFF0000);
    return c;
  }
#endif

}


