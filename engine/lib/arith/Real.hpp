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
#include "fixed/Fixed32.hpp"

#define UE_OPTION_ARITHMETIC_FLOAT

#define R(x) ((Real){x})

namespace ue {

class Real
{
public:
//USING FLOATING POINT OPERATIONS
#ifdef UE_OPTION_ARITHMETIC_FLOAT
	float val;
//USING 32bits FIXED POINT OPERATIONS
#else
#ifdef UE_OPTION_ARITHMETIC_FIXED32
	Fixed32 val;
#endif //UE_OPTION_ARITHMETIC_FIXED32
#endif //UE_OPTION_ARITHMETIC_FLOAT

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

	//****Conversion****
	void toStr(char*);//Write to the buffer size without limit
	void toStr(char*, uint32_t);//Write to the buffer size with limit

};
}

#endif /* REAL_H_ */
