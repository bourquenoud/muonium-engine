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
#define UE_REAL_MAX (R(1000))
#define UE_REAL_MIN (R(-1000))
#elif UE_CONFIG_ARITHMETIC == FIXED32
#define UE_REAL_MAX (R(1000))
#define UE_REAL_MIN (R(-1000))
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

    //****Arithmetic****
    Real operator+(); //Unary +
    Real operator-(); //Unary -
    Real operator+(Real);//add
    Real operator-(Real);//sub
    Real operator*(Real);//mult
    Real operator/(Real);//div
    Real operator%(Real);//mod

    //****Inc/dec****
    Real operator++();//pre inc
    Real operator--();//pre dec
    Real operator++(int);//post inc
    Real operator--(int);//post dec

    //****Assignment****
    Real operator=(Real);//assign
    Real operator+=(Real);//add
    Real operator-=(Real);//sub
    Real operator*=(Real);//mult
    Real operator/=(Real);//div
    Real operator%=(Real);//mod

    //****Comparison****
    bool operator==(Real);//equal
    bool operator!=(Real);//not equal
    bool operator<(Real);//smaller
    bool operator>(Real);//bigger
    bool operator<=(Real);//smaller or equal
    bool operator>=(Real);//bigger or equal

    //****typecast****
    operator int8_t();
    operator int16_t();
    operator int32_t();
    operator uint8_t();
    operator uint16_t();
    operator uint32_t();
    operator float();

    //****basic arithmetics****
    static Real min(Real,Real);
    static Real min(Real,Real,Real);
    static Real max(Real,Real);
    static Real max(Real,Real,Real);
    static Real abs(Real);

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
