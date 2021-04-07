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
#include "engine/ue_config.hpp"
#include "fixed/Fixed32.hpp"

namespace ue {

  class Real
  {
  public:
    //USING FLOATING POINT OPERATIONS
#ifdef UE_CONFIG_ARITHMETIC_FLOAT
    float val;
    //USING 32bits FIXED POINT OPERATIONS
#else
#ifdef UE_CONFIG_ARITHMETIC_FIXED32
    Fixed32 val;
#endif //UE_CONFIG_ARITHMETIC_FIXED32
#endif //UE_CONFIG_ARITHMETIC_FLOAT

    //****Constructor****
    Real();
    Real(int);
    Real(float);
#ifdef UE_CONFIG_CPP17_SYNTAX
    constexpr Real(long double a) : val(a)
    {
      val = (float)a;
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
    operator float();

  };
}

//Literal definition
#ifdef UE_CONFIG_CPP17_SYNTAX
#define R(X) (X##_r)
constexpr ue::Real operator"" _r(long double a)
{
  return ue::Real(a);
}
#else
#define R(X) ((Real)X)
#endif //UE_CONFIG_CPP17_SYNTAX

#endif /* REAL_H_ */
