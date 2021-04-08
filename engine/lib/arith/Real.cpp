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


  /****Arithmetic****/
  Real Real::operator+() //Unary +
  {
          Real temp;
          temp.val = val;
          return temp;
  }
  Real Real::operator-() //Unary -
  {
          Real temp;
          temp.val = -val;
          return temp;
  }
  Real Real::operator+(Real a)//add
  {
          Real temp;
          temp.val = val + a.val;
          return temp;
  }
  Real Real::operator-(Real a)//sub
  {
          Real temp;
          temp.val = val - a.val;
          return temp;
  }
  Real Real::operator*(Real a)//mult
  {
          Real temp;
          temp.val = val * a.val;
          return temp;
  }
  Real Real::operator/(Real a)//div
  {
          Real temp;
          temp.val = val / a.val;
          return temp;
  }
  /*
  Real Real::operator%(Real a)//mod
  {
          Real temp;
          temp.val = val % a.val;
          return temp;
  }*/

  /****Inc/dec****/
  Real Real::operator++()
  {
          Real temp;
          temp.val = ++val;
          return temp;
  }
  Real Real::operator--()//pre dec
  {
          Real temp;
          temp.val = --val;
          return temp;
  }
  Real Real::operator++(int a)//post inc
  {
          Real temp;
          temp.val = val++;
          return temp;
  }
  Real Real::operator--(int a)//post dec
  {
          Real temp;
          temp.val = val--;
          return temp;
  }

  //****Assignment****//
  Real Real::operator=(Real a)//assign
  {
          this->val = a.val;
          return *this;
  }
  Real Real::operator+=(Real a)//add
  {
          this->val = val + a.val;
          return *this;
  }
  Real Real::operator-=(Real a)//sub
  {
          this->val = val - a.val;
          return *this;
  }
  Real Real::operator*=(Real a)//mult
  {
          this->val = val * a.val;
          return *this;
  }
  Real Real::operator/=(Real a)//div
  {
          this->val = val / a.val;
          return *this;
  }
  /*Real Real::operator%=(Real a)//mod
  {
          Real temp;
          temp.val = val % a.val;
          return temp;
  }*/

  //****Comparison****//
  bool Real::operator==(Real a)//equal
  {
   return (val == a.val);
  }
  bool Real::operator!=(Real a)//not equal
  {
           return (val != a.val);
  }
  bool Real::operator<(Real a)//smaller
  {
           return (val < a.val);
  }
  bool Real::operator>(Real a)//bigger
  {
           return (val > a.val);
  }
  bool Real::operator<=(Real a)//smaller or equal
  {
           return (val <= a.val);
  }
  bool Real::operator>=(Real a)//bigger or equal
  {
           return (val >= a.val);
  }

  //****Typecast****//
  Real::operator int8_t()
  {
          return (int8_t)(this->val);
  }
  Real::operator int16_t()
  {
          return (int16_t)(this->val);
  }
  Real::operator int32_t()
  {
          return (int32_t)(this->val);
  }
  Real::operator float()
  {
    return (float)(this->val);
  }

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
  Real Real::abs(Real a)
  {
    return (a < R(0))?-a:a;
  }

}


