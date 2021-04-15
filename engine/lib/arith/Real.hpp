/*
 * Real.h
 *
 * We overload all floating point operations to make it transparent to use either fixed or floating point
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */
//TODO:
// - toStr
// - Make the R macro adapt to Fixed point
// - Implement trigo
// - Implement sqrt
// - Implement fast math

#ifndef REAL_H_
#define REAL_H_

#include <cstdint>
#include <cfloat>
#include <cmath>
#include "engine/ue_config.hpp"
#include "fixed/Fixed32.hpp"

/* FIXME
#if UE_CONFIG_ARITHMETIC == FLOAT
#define UE_REAL_MAX (R(FLT_MAX))
#define UE_REAL_MIN (R(FLT_MAX))
#elif UE_CONFIG_ARITHMETIC == FIXED32
#define UE_REAL_MAX ((Real){(Fixed32){0x7FFFFFFF}})
#define UE_REAL_MIN ((Real){(Fixed32){0x80000000}})
#endif //UE_CONFIG_ARITHMETIC
 */
//XXX BAD XXX
#if UE_CONFIG_ARITHMETIC == FLOAT
#define UE_REAL_MAX (R(10000))
#define UE_REAL_MIN (R(-10000))
#elif UE_CONFIG_ARITHMETIC == FIXED32
#define UE_REAL_MAX (R(10000))
#define UE_REAL_MIN (R(-10000))
#endif //UE_CONFIG_ARITHMETIC

namespace ue {

  class Real
  {
  public:
#if UE_CONFIG_ARITHMETIC == FLOAT
    float val;
#elif UE_CONFIG_ARITHMETIC == FIXED32
    Fixed32 val;
#endif //UE_CONFIG_ARITHMETIC

    //****Constructor****
    Real();
    Real(int);
    Real(unsigned int);
    Real(float);
#if __cplusplus >= 201703L
    constexpr Real(long double a) : val(a)
    {
      val = (float)a;
    }
#elif UE_CONFIG_ARITHMETIC == FLOAT
    Real(double a)
    {
      val = (float)a;
    }
#elif UE_CONFIG_ARITHMETIC == FIXED32
    Real(double a)
    {
      val = (Fixed32)(float)a;
    }
#endif //UE_CONFIG_CPP17_SYNTAX


    /****Arithmetic****/
    inline Real operator+() //Unary +
    {
      Real temp;
      temp.val = val;
      return temp;
    }
    inline Real operator-() //Unary -
    {
      Real temp;
      temp.val = -val;
      return temp;
    }
    inline Real operator+(Real a)//add
    {
      Real temp;
      temp.val = val + a.val;
      return temp;
    }
    inline Real operator-(Real a)//sub
    {
      Real temp;
      temp.val = val - a.val;
      return temp;
    }
    inline Real operator*(Real a)//mult
    {
      Real temp;
      temp.val = val * a.val;
      return temp;
    }
    inline Real operator/(Real a)//div
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
    inline Real operator--()//pre dec
                {
      Real temp;
      temp.val = --val;
      return temp;
                }
    inline Real operator++(int a)//post inc
                {
      Real temp;
      temp.val = val++;
      return temp;
                }
    inline Real operator--(int a)//post dec
                {
      Real temp;
      temp.val = val--;
      return temp;
                }

    //****Assignment****//
    inline Real operator=(Real a)//assign
    {
      this->val = a.val;
      return *this;
    }
    inline Real operator+=(Real a)//add
                {
      this->val = val + a.val;
      return *this;
                }
    inline Real operator-=(Real a)//sub
                {
      this->val = val - a.val;
      return *this;
                }
    inline Real operator*=(Real a)//mult
                {
      this->val = val * a.val;
      return *this;
                }
    inline Real operator/=(Real a)//div
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
    inline bool operator==(Real a)//equal
                {
      return (val == a.val);
                }
    inline bool operator!=(Real a)//not equal
                {
      return (val != a.val);
                }
    inline bool operator<(Real a)//smaller
    {
      return (val < a.val);
    }
    inline bool operator>(Real a)//bigger
    {
      return (val > a.val);
    }
    inline bool operator<=(Real a)//smaller or equal
                {
      return (val <= a.val);
                }
    inline bool operator>=(Real a)//bigger or equal
                {
      return (val >= a.val);
                }

    //****Typecast****//
    inline operator int8_t()
                {
      return (int8_t)(this->val);
                }
    inline operator int16_t()
                {
      return (int16_t)(this->val);
                }
    inline operator int32_t()
                {
      return (int32_t)(this->val);
                }
    inline operator uint8_t()
                {
      return (uint8_t)(val);
                }
    inline operator uint16_t()
                {
      return (uint16_t)(val);
                }
    inline operator uint32_t()
                {
      return (uint32_t)(val);
                }
    inline operator float()
                {
      return (float)(val);
                }

    //****basic arithmetics****
    static Real min(Real,Real);
    static Real min(Real,Real,Real);
    static Real max(Real,Real);
    static Real max(Real,Real,Real);
    static Real clamp(Real,Real,Real);
    static Real abs(Real);
    //TODO: Check if it works for Fixed32 with negatives
    static Real floor(Real);
    static Real ceil(Real);
    static Real round(Real);

  };
}

//Literal definition
#if __cplusplus >= 201703L
#define R(X) (X##_r)
constexpr ue::Real operator"" _r(long double a)
    {
  return ue::Real(a);
    }
#else
#define R(X) ((ue::Real)X)
#endif //UE_CONFIG_CPP17_SYNTAX

#endif /* REAL_H_ */
