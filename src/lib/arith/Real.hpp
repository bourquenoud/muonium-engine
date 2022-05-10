/*
 * Real.h
 *
 * We overload all floating point operations to make it transparent to use either fixed or floating point
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */
// TODO:
//  - toStr
//  - Make the R macro adapt to Fixed point
//  - Implement trigo
//  - Implement sqrt
//  - Implement fast math

#ifndef REAL_H_
#define REAL_H_

#include <cstdint>
#include <cfloat>
#include <cmath>

#include "fixed/Fixed32.hpp"
#include "../../ue_config.hpp"

/* FIXME
#if UE_CONFIG_ARITHMETIC == FLOAT
#define UE_REAL_MAX (R(FLT_MAX))
#define UE_REAL_MIN (R(FLT_MAX))
#elif UE_CONFIG_ARITHMETIC == FIXED32
#define UE_REAL_MAX ((Real){(Fixed32){0x7FFFFFFF}})
#define UE_REAL_MIN ((Real){(Fixed32){0x80000000}})
#endif //UE_CONFIG_ARITHMETIC
 */
// XXX BAD XXX
#if UE_CONFIG_ARITHMETIC == FLOAT
#define UE_REAL_MAX (R(10000.0))
#define UE_REAL_MIN (R(-10000.0))
#elif UE_CONFIG_ARITHMETIC == FIXED32
#define UE_REAL_MAX (R(10000.0))
#define UE_REAL_MIN (R(-10000.0))
#endif // UE_CONFIG_ARITHMETIC

namespace ue
{

  class Real
  {
  public:
#if UE_CONFIG_ARITHMETIC == FLOAT
    float val;
#elif UE_CONFIG_ARITHMETIC == FIXED32
    Fixed32 val;
#endif // UE_CONFIG_ARITHMETIC

    //****Constructor****
    /****Constructor****/ // USING FLOATING POINT OPERATIONS
#if UE_CONFIG_ARITHMETIC == FLOAT
    inline Real()
    {
    }
    inline Real(const int a)
    {
      val = (float)a;
    }
    inline Real(const unsigned int a)
    {
      val = (float)a;
    }
    inline Real(const char a)
    {
      val = (float)a;
    }
    inline Real(const unsigned char a)
    {
      val = (float)a;
    }
    inline Real(const float a)
    {
      val = a;
    }
    inline Real(const double a)
    {
      val = (float)a;
    }
    // USING 32bits FIXED POINT OPERATIONS
#elif UE_CONFIG_ARITHMETIC == FIXED32
    inline Real()
    {
    }
    inline Real(const int a)
    {
      val = (Fixed32)a;
    }
    inline Real(const unsigned int a)
    {
      val = (Fixed32)a;
    }
    inline Real(const float a)
    {
      val = (Fixed32)a;
    }
    inline Real(const double a)
    {
      val = (Fixed32)a;
    }
#endif // UE_CONFIG_ARITHMETIC

#if __cplusplus >= 201703L && UE_CONFIG_ARITHMETIC == FLOAT
    constexpr Real(const long double a) : val(a)
    {
      val = (float)a;
    }
#elif __cplusplus >= 201703L && UE_CONFIG_ARITHMETIC == FIXED32
    constexpr Real(const long double a) : val(a)
    {
      val = (Fixed32)a;
    }
#elif UE_CONFIG_ARITHMETIC == FLOAT
    inline Real(const double a)
    {
      val = (float)a;
    }
#elif UE_CONFIG_ARITHMETIC == FIXED32
    inline Real(const double a)
    {
      val = (Fixed32)(float)a;
    }
#endif // UE_CONFIG_CPP17_SYNTAX

    /****Arithmetic****/
    inline Real operator+() const // Unary +
    {
      Real temp;
      temp.val = val;
      return temp;
    }
    inline Real operator-() const // Unary -
    {
      Real temp;
      temp.val = -val;
      return temp;
    }
    inline Real operator+(const Real a) const // add
    {
      Real temp;
      temp.val = val + a.val;
      return temp;
    }
    inline Real operator-(const Real a) const // sub
    {
      Real temp;
      temp.val = val - a.val;
      return temp;
    }
    inline Real operator*(const Real a) const // mult
    {
      Real temp;
      temp.val = val * a.val;
      return temp;
    }
    inline Real operator/(const Real a) const // div
    {
      Real temp;
      temp.val = val / a.val;
      return temp;
    }
    /*
    Real operator%(Real a)//mod
    {
            Real temp;
            temp.val = val % a.val;
            return temp;
    }*/

    /****Inc/dec****/
    inline Real operator++()
    {
      Real temp;
      temp.val = ++val;
      return temp;
    }
    inline Real operator--() // pre dec
    {
      Real temp;
      temp.val = --val;
      return temp;
    }
    inline Real operator++(int a) // post inc
    {
      Real temp;
      temp.val = val++;
      return temp;
    }
    inline Real operator--(int a) // post dec
    {
      Real temp;
      temp.val = val--;
      return temp;
    }

    //****Assignment****//
    inline Real operator=(Real a) // assign
    {
      this->val = a.val;
      return *this;
    }
    inline Real operator+=(Real a) // add
    {
      this->val = val + a.val;
      return *this;
    }
    inline Real operator-=(Real a) // sub
    {
      this->val = val - a.val;
      return *this;
    }
    inline Real operator*=(Real a) // mult
    {
      this->val = val * a.val;
      return *this;
    }
    inline Real operator/=(Real a) // div
    {
      this->val = val / a.val;
      return *this;
    }
    /*Real operator%=(Real a)//mod
    {
            Real temp;
            temp.val = val % a.val;
            return temp;
    }*/

    //****Comparison****//
    inline bool operator==(Real a) // equal
    {
      return (val == a.val);
    }
    inline bool operator!=(Real a) // not equal
    {
      return (val != a.val);
    }
    inline bool operator<(Real a) // smaller
    {
      return (val < a.val);
    }
    inline bool operator>(Real a) // bigger
    {
      return (val > a.val);
    }
    inline bool operator<=(Real a) // smaller or equal
    {
      return (val <= a.val);
    }
    inline bool operator>=(Real a) // bigger or equal
    {
      return (val >= a.val);
    }

    //****Typecast****//
    inline operator int8_t() const
    {
      return (int8_t)(this->val);
    }
    inline operator int16_t() const
    {
      return (int16_t)(this->val);
    }
    inline operator int32_t() const
    {
      return (int32_t)(this->val);
    }
    inline operator uint8_t() const
    {
      return (uint8_t)(val);
    }
    inline operator uint16_t() const
    {
      return (uint16_t)(val);
    }
    inline operator uint32_t() const
    {
      return (uint32_t)(val);
    }
    inline operator float() const
    {
      return (float)(val);
    }

    //****basic arithmetics****
    static Real min(Real, Real);
    static Real min(Real, Real, Real);
    static Real max(Real, Real);
    static Real max(Real, Real, Real);
    static Real clamp(Real, Real, Real);
    static Real abs(Real);
    // TODO: Check if it works for Fixed32 with negatives
#if UE_CONFIG_ARITHMETIC == FLOAT
    static inline Real floor(Real a)
    {
      Real c;
      c.val = floorf((float)a.val);
      return c;
    }
    static inline Real ceil(Real a)
    {
      Real c;
      c.val = ceilf((float)a.val);
      return c;
    }
    static inline Real round(Real a)
    {
      Real c;
      c.val = roundf((float)a.val);
      return c;
    }
#elif UE_CONFIG_ARITHMETIC == FIXED32
    static inline Real floor(Real a)
    {
      Real c;
      // Mask the fractional part
      c.val.val = a.val.val & 0xFFFF0000;
      return c;
    }
    static inline Real ceil(Real a)
    {
      Real c;
      //+1 then floor except for round numbers
      c.val.val = ((a.val.val + 0x0000FFFF) & 0xFFFF0000);
      return c;
    }
    static inline Real round(Real a)
    {
      Real c;
      //+0.5 then floor
      c.val.val = ((a.val.val + 0x00008000) & 0xFFFF0000);
      return c;
    }
#endif
  };
}

#if __cplusplus >= 201703L
constexpr ue::Real operator""_r(long double a)
{
  return ue::Real(a);
}
#define R(X) (X##_r)
#else
#define R(X) ((ue::Real)(X))
#endif // UE_CONFIG_CPP17_SYNTAX

#endif /* REAL_H_ */
