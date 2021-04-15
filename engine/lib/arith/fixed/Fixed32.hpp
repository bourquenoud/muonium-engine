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

    constexpr Fixed32(long double a) : val(a)
    {
      val = (int32_t)round(a * (1<<16));
    }
    //****Constructor****
    inline Fixed32()
    {

    }
    inline Fixed32(const uint32_t a)
    {
      val = (a<<16);
    }
    inline Fixed32(const int32_t a)
    {
      val = (a<<16);
    }
    inline Fixed32(const float a)
    {
      val = (int32_t)roundf(a * (1<<16));
    }
    //****Arithmetic****
    inline Fixed32 operator+() //Unary +
    {
      Fixed32 temp;
      temp.val = val;
      return temp;
    }

    inline Fixed32 operator-() //Unary -
    {
      Fixed32 temp;
      temp.val = -val;
      return temp;
    }
    inline Fixed32 operator+(Fixed32 a)//add
    {
      Fixed32 temp;
      temp.val = val + a.val;
      return temp;
    }
    inline Fixed32 operator-(Fixed32 a)//sub
    {
      Fixed32 temp;
      temp.val = val - a.val;
      return temp;
    }
    inline Fixed32 operator*(Fixed32 a)//mult
    {
      Fixed32 temp;
      temp.val = ((int64_t)val * (int64_t)a.val) >> 16;
      return temp;
    }
    inline Fixed32 operator/(Fixed32 a)//div
    {
      Fixed32 temp;
      temp.val = ((int64_t)val << 16) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
      return temp;
    }
    inline Fixed32 operator%(Fixed32 a)//mod
    {
      Fixed32 temp;
      temp.val = val % a.val;
      return temp;
    }

    //****Inc/dec****//
    inline Fixed32 operator++()//pre inc
          {
      Fixed32 temp;
      temp.val = (val + 0x10000);
      return temp;
          }
    inline Fixed32 operator--()//pre dec
          {
      Fixed32 temp;
      temp.val = (val - 0x10000);
      return temp;
          }
    inline Fixed32 operator++(int a)//post inc
          {
      Fixed32 temp;
      temp.val = val;
      val = (val + 0x10000);
      return temp;
          }
    inline Fixed32 operator--(int a)//post dec
          {
      Fixed32 temp;
      temp.val = val;
      val = (val - 0x10000);
      return temp;
          }

    //****Assignment****
    inline Fixed32 operator=(Fixed32 a)//assign
    {
      this->val = a.val;
      return a;
    }
    inline Fixed32 operator+=(Fixed32 a)//add
          {
      Fixed32 temp;
      temp.val = val + a.val;
      return temp;
          }
    inline Fixed32 operator-=(Fixed32 a)//sub
          {
      Fixed32 temp;
      temp.val = val - a.val;
      this->val = temp.val;
      return temp;
          }
    inline Fixed32 operator*=(Fixed32 a)//mult
          {
      Fixed32 temp;
      temp.val = ((int64_t)val * (int64_t)a.val) >> 16;
      this->val = temp.val;
      return temp;
          }
    inline Fixed32 operator/=(Fixed32 a)//div
          {
      Fixed32 temp;
      temp.val = ((int64_t)val << 16) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
      this->val = temp.val;
      return temp;
          }
    inline Fixed32 operator%=(Fixed32 a)//mod
          {
      Fixed32 temp;
      temp.val = val % a.val;
      this->val = temp.val;
      return temp;
          }

    //****Comparison****
    inline bool operator==(Fixed32 a)//equal
          {
      return (val == a.val);
          }
    inline bool operator!=(Fixed32 a)//not equal
          {
      return (val != a.val);
          }
    inline bool operator<(Fixed32 a)//smaller
    {
      return (val < a.val);
    }
    inline bool operator>(Fixed32 a)//bigger
    {
      return (val > a.val);
    }
    inline bool operator<=(Fixed32 a)//smaller or equal
          {
      return (val <= a.val);
          }
    inline bool operator>=(Fixed32 a)//bigger or equal
          {
      return (val >= a.val);
          }

    //****Typecast****
    inline operator int8_t()
          {
      return (int8_t)(this->val>>16);
          }
    inline operator int16_t()
          {
      return (int16_t)(this->val>>16);
          }
    inline operator int32_t()
          {
      return (int32_t)(this->val>>16);
          }
    inline operator uint8_t()
          {
      return (uint8_t)(val>>16);
          }
    inline operator uint16_t()
          {
      return (uint16_t)(val>>16);
          }
    inline operator uint32_t()
          {
      return (uint32_t)(val>>16);
          }
    inline operator float()
          {
      return (float)(val) / (1<<16);
          }
  };

}

#endif /* FIXED32_H_ */
