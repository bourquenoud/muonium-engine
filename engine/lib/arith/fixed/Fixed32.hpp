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

namespace ue
{

class Fixed32
{
	int32_t val;

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
};

}

#endif /* FIXED32_H_ */
