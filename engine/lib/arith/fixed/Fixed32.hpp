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

#include "../../../ue_config.hpp"

#define UE_OVERFLOW true


#if UE_OVERFLOW
#include <csignal>
#include <stdio.h>

static void myfunc3(void)
{
	fprintf(stderr, "Backtrace\n");
}

#define UE_DEBUG_CHECK_OVERFLOW(x) if(x) {fprintf(stderr, "ERROR : Overflow occurred at line %i\n", __LINE__); myfunc3();}

#else
#define UE_DEBUG_CHECK_OVERFLOW(x)
#define HALF (1<<31)
#endif

namespace ue
{

class Fixed32
{
public:
	int32_t val;

	constexpr Fixed32(long double a) : val(a)
	{
		val = (int32_t)round(a * (1<<UE_CONFIG_FIXED32_DECIMALS));
	}
	//****Constructor****
	inline Fixed32()
	{

	}
	inline Fixed32(const uint32_t& a)
	{
		val = (a<<UE_CONFIG_FIXED32_DECIMALS);
	}
	inline Fixed32(const int32_t& a)
	{
		val = (a<<UE_CONFIG_FIXED32_DECIMALS);
	}
	inline Fixed32(const float& a)
	{
		val = (int32_t)roundf(a * (1<<UE_CONFIG_FIXED32_DECIMALS));
	}
	inline Fixed32(const double& a)
	{
		val = (int32_t)roundf(a * (1<<UE_CONFIG_FIXED32_DECIMALS));
	}
	//****Arithmetic****
	inline Fixed32 operator+() //Unary +
	{
		Fixed32 temp;
		temp.val = val;
		return temp;
	}

	inline Fixed32 operator-() const //Unary -
	{
		Fixed32 temp;
		temp.val = -val;
		return temp;
	}
	inline Fixed32 operator+(Fixed32 a) const//add
	{

		Fixed32 temp;
		temp.val = val + a.val;
		UE_DEBUG_CHECK_OVERFLOW((int64_t)val + (int64_t)a.val != (int64_t)temp.val);
		return temp;
	}
	inline Fixed32 operator-(Fixed32 a) const//sub
	{
		Fixed32 temp;
		temp.val = val - a.val;
		UE_DEBUG_CHECK_OVERFLOW((int64_t)val - (int64_t)a.val != (int64_t)temp.val);
		return temp;
	}
	inline Fixed32 operator*(Fixed32 a) const//mult
	{
		Fixed32 temp;
		temp.val = ((int64_t)val * (int64_t)a.val) >> UE_CONFIG_FIXED32_DECIMALS;
		UE_DEBUG_CHECK_OVERFLOW(abs(((int64_t)val * (int64_t)a.val) >> UE_CONFIG_FIXED32_DECIMALS) & (0xFFFFFFFF00000000));
		return temp;
	}

	//XXX: this is the most time wasting function
	inline Fixed32 operator/(Fixed32 a) const//div
	{
		Fixed32 temp;
		temp.val = ((int64_t)val << UE_CONFIG_FIXED32_DECIMALS) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
		//temp.val = (val / (a.val >> 8)) << 8; //Half precision (Really sucks)
		return temp;
	}
	inline Fixed32 operator%(Fixed32 a) const//mod
	{
		Fixed32 temp;
		temp.val = val % a.val;
		return temp;
	}

	//****Inc/dec****//
	inline Fixed32 operator++()//pre inc
        						  {
		Fixed32 temp;
		temp.val = (val + (1 << UE_CONFIG_FIXED32_DECIMALS));
		return temp;
        						  }
	inline Fixed32 operator--()//pre dec
        						  {
		Fixed32 temp;
		temp.val = (val - (1 << UE_CONFIG_FIXED32_DECIMALS));
		return temp;
        						  }
	inline Fixed32 operator++(int a)//post inc
        						  {
		Fixed32 temp;
		temp.val = val;
		val = (val + (1 << UE_CONFIG_FIXED32_DECIMALS));
		return temp;
        						  }
	inline Fixed32 operator--(int a)//post dec
        						  {
		Fixed32 temp;
		temp.val = val;
		val = (val - (1 << UE_CONFIG_FIXED32_DECIMALS));
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
		temp.val = ((int64_t)val * (int64_t)a.val) >> UE_CONFIG_FIXED32_DECIMALS;
		this->val = temp.val;
		return temp;
        						  }
	inline Fixed32 operator/=(Fixed32 a)//div
        						  {
		Fixed32 temp;
		temp.val = ((int64_t)val << UE_CONFIG_FIXED32_DECIMALS) / a.val; // S*(x*S)/(y*S) = x/y*S with S the scaling factor, x dividend and y divisor
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
	inline operator int8_t() const
        						  {
		return (int8_t)(this->val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator int16_t() const
        						  {
		return (int16_t)(this->val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator int32_t() const
        						  {
		return (int32_t)(this->val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator uint8_t() const
        						  {
		return (uint8_t)(val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator uint16_t() const
        						  {
		return (uint16_t)(val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator uint32_t() const
        						  {
		return (uint32_t)(val>>UE_CONFIG_FIXED32_DECIMALS);
        						  }
	inline operator float() const
        						  {
		return (float)(val) / (1<<UE_CONFIG_FIXED32_DECIMALS);
        						  }
};

}

#endif /* FIXED32_H_ */
