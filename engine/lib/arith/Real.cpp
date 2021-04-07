/*
 * Real.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Real.hpp"

namespace ue
{
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
	temp.val = val + a.val;
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
	this->val = val + a;
	return *this;
}
Real Real::operator-=(Real a)//sub
{
	this->val = val - a;
	return *this;
}
Real Real::operator*=(Real a)//mult
{
	this->val = val * a;
	return *this;
}
Real Real::operator/=(Real a)//div
{
	this->val = val / a;
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
#ifdef UE_OPTION_ARITHMETIC_FLOAT
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
#else
#ifdef UE_OPTION_ARITHMETIC_FIXED32
Real::operator int8_t()
{
	return (int8_t)(this->val >> 16);
}
Real::operator int16_t()
{
	return (int16_t)(this->val >> 16);
}
Real::operator int32_t()
{
	return (int32_t)(this->val >> 16);
}
#endif //UE_OPTION_ARITHMETIC_FIXED32
#endif //UE_OPTION_ARITHMETIC_FLOAT


void Real::toStr(char* c)//Write to the buffer without size limit
{
	/*
	char* ptr = c;
	Real temp = *this;
	bool sign = (temp > R(0));
	if(!sign)
	{
		temp = -temp;
		*(ptr++) = '-';
	}

	while(temp > R(0))
	{
		*(ptr++) = ((temp %) + );
	}*/
}
void Real::toStr(char* c, uint32_t len)//Write to the buffer with size limit
{

}
}


