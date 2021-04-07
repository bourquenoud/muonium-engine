/*
 * Fixed32.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Fixed32.hpp"

namespace ue
{
//****Arithmetic****
Fixed32 Fixed32::operator+() //Unary +
{
	Fixed32 temp;
	temp.val = val;
	return temp;
}
Fixed32 Fixed32::operator-() //Unary -
{
	Fixed32 temp;
	temp.val = -val;
	return temp;
}
Fixed32 Fixed32::operator+(Fixed32 a)//add
{
	Fixed32 temp;
	temp.val = val + a.val;
	return temp;
}
Fixed32 Fixed32::operator-(Fixed32 a)//sub
{
	Fixed32 temp;
	temp.val = val - a.val;
	return temp;
}
Fixed32 Fixed32::operator*(Fixed32 a)//mult
{
	Fixed32 temp;
	temp.val = ((int64_t)val * (int64_t)a.val) >> 16;
	return temp;
}
Fixed32 Fixed32::operator/(Fixed32 a)//div
{
	Fixed32 temp;
	temp.val = ((int64_t)val << 16) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
	return temp;
}
Fixed32 Fixed32::operator%(Fixed32 a)//mod
{
	Fixed32 temp;
	temp.val = val % a.val;
	return temp;
}

//****Inc/dec****//
Fixed32 Fixed32::operator++()//pre inc
{
	Fixed32 temp;
	temp.val = (val + 0x10000);
	return temp;
}
Fixed32 Fixed32::operator--()//pre dec
{
	Fixed32 temp;
	temp.val = (val - 0x10000);
	return temp;
}
Fixed32 Fixed32::operator++(int a)//post inc
{
	Fixed32 temp;
	temp.val = val;
	val = (val + 0x10000);
	return temp;
}
Fixed32 Fixed32::operator--(int a)//post dec
{
	Fixed32 temp;
	temp.val = val;
	val = (val - 0x10000);
	return temp;
}

//****Assignment****
Fixed32 Fixed32::operator=(Fixed32 a)//assign
{
	return a;
}
Fixed32 Fixed32::operator+=(Fixed32 a)//add
{
	Fixed32 temp;
	temp.val = val + a.val;
	return temp;
}
Fixed32 Fixed32::operator-=(Fixed32 a)//sub
{
	Fixed32 temp;
	temp.val = val - a.val;
	return temp;
}
Fixed32 Fixed32::operator*=(Fixed32 a)//mult
{
	Fixed32 temp;
	temp.val = ((int64_t)val * (int64_t)a.val) >> 16;
	return temp;
}
Fixed32 Fixed32::operator/=(Fixed32 a)//div
{
	Fixed32 temp;
	temp.val = ((int64_t)val << 16) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
	return temp;
}
Fixed32 Fixed32::operator%=(Fixed32 a)//mod
{
	Fixed32 temp;
	temp.val = val % a.val;
	return temp;
}

//****Comparison****
bool Fixed32::operator==(Fixed32 a)//equal
{
 return (val == a.val);
}
bool Fixed32::operator!=(Fixed32 a)//not equal
{
	 return (val != a.val);
}
bool Fixed32::operator<(Fixed32 a)//smaller
{
	 return (val < a.val);
}
bool Fixed32::operator>(Fixed32 a)//bigger
{
	 return (val > a.val);
}
bool Fixed32::operator<=(Fixed32 a)//smaller or equal
{
	 return (val <= a.val);
}
bool Fixed32::operator>=(Fixed32 a)//bigger or equal
{
	 return (val >= a.val);
}
}
