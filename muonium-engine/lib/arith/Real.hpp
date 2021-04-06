/*
 * Real.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef REAL_H_
#define REAL_H_

namespace ue {

//-----------------------------------------------------------
//USING FLOATING POINT OPERATIONS
//-----------------------------------------------------------
#ifdef UE_OPTION_ARITHMETIC_FLOAT
class Real
{

};
//-----------------------------------------------------------
//USING 32bits FIXED POINT OPERATIONS
//-----------------------------------------------------------
#elif UE_OPTION_ARITHMETIC_FIXED32
class Real
{

};
#endif
/*
//-----------------------------------------------------------
//USING 16bits FIXED POINT OPERATIONS
// (not sure it I will implement that)
//-----------------------------------------------------------
#elif UE_OPTION_ARITHMETIC_FIXED16
class Real
{

};
#endif
*/
}

#endif /* REAL_H_ */
