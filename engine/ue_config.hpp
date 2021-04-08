/*
 * ue_config.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef UE_CONFIG_H_
#define UE_CONFIG_H_

#include "ue_const.hpp"

#define UE_CONFIG_ARITHMETIC_FIXED32
//#define UE_CONFIG_ARITHMETIC_FLOAT
#define UE_CONFIG_TEXTURE
#define UE_CONFIG_NORMAL

/*
 * Define the colour mode of the buffer.
 *
 * Colour mode available
 * 32bits : ARGB8888, RGBA8888
 * 16bits : ARGB1555, RGBA5551, RGB565
 * 8bits : RGB332, MONO8
 */
#define UE_CONFIG_COLOUR ARGB8888


#endif /* UE_CONFIG_H_ */
