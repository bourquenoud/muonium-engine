/*
 * Fixed32.h
 *
 * Fixed point arithmetic to speed up calculations on MCU without FPU. 16bits integer and 16 bits fractional
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

//TODO:
// - Implement trigo
// - Implement sqrt
// - Implement fast math

#ifndef FIXED32_H_
#define FIXED32_H_

#include <cstdint>
#include <cmath>

namespace ue
{

  class Fixed32
  {
  public:
    int32_t val;

    //****Constructor****
    Fixed32();
    Fixed32(int32_t);
    Fixed32(uint32_t);
    Fixed32(float);
    constexpr Fixed32(long double a) : val(a)
    {
      val = (int32_t)round(a * (1<<16));
    }

    //****Arithmetic****
    Fixed32 operator+(); //Unary +
    Fixed32 operator-(); //Unary -
    Fixed32 operator+(Fixed32);//add
    Fixed32 operator-(Fixed32);//sub
    Fixed32 operator*(Fixed32);//mult
    Fixed32 operator/(Fixed32);//div
    Fixed32 operator%(Fixed32);//mod

    //****Inc/dec****
    Fixed32 operator++();//pre inc
    Fixed32 operator--();//pre dec
    Fixed32 operator++(int);//post inc
    Fixed32 operator--(int);//post dec

    //****Assignment****
    Fixed32 operator=(Fixed32);//assign
    Fixed32 operator+=(Fixed32);//add
    Fixed32 operator-=(Fixed32);//sub
    Fixed32 operator*=(Fixed32);//mult
    Fixed32 operator/=(Fixed32);//div
    Fixed32 operator%=(Fixed32);//mod

    //****Comparison****
    bool operator==(Fixed32);//equal
    bool operator!=(Fixed32);//not equal
    bool operator<(Fixed32);//smaller
    bool operator>(Fixed32);//bigger
    bool operator<=(Fixed32);//smaller or equal
    bool operator>=(Fixed32);//bigger or equal

    //****Typecast****
    operator int8_t();
    operator int16_t();
    operator int32_t();
    operator uint8_t();
    operator uint16_t();
    operator uint32_t();
    operator float();
  };

}

#endif /* FIXED32_H_ */
