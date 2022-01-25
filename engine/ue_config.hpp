/*
 * ue_config.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef UE_CONFIG_H_
#define UE_CONFIG_H_

#include "ue_const.hpp"

/*
 * Define the arithmetic type used.
 *
 * To speed up calculations for MCU without FPU, it is possible
 * to enable a fixed point arithmetic mode. Objects have to be properly
 * scaled before.
 * The number of decimal is defined by UE_CONFIG_FIXED32_DECIMALS.
 *
 * Mode available
 * FLOAT
 * FIXED32
 */
#define UE_CONFIG_ARITHMETIC FLOAT

/*
 * Define the number of decimals used for the 32 bits fixed point arithmetic
 *
 * You should probably not change this. the default value is 16
 *
 * Value available
 * 0 to 32
 *
 */
#define UE_CONFIG_FIXED32_DECIMALS 16

/*
 * Enable render texture
 *
 * Rendering textures makes the computation a bit slower and uses more memory.
 *
 * true or false
 */
#define UE_CONFIG_ENABLE_TEXTURE true

/*
 * Enable normals
 *
 * Interpolate normals to make a smoother surface. Enabling this will disable
 * light precomputing and makes the rendering slower. Disabling it will make the
 * object angle sharp
 *
 * true or false
 */
#define UE_CONFIG_ENABLE_SMOOTH_NORMAL true

/*
 * Define the colour mode of the buffer. No significant change in speed between
 * the 16 and 32 bits format. It reduces memory usage.
 *
 * Colour mode available
 * 32bits : ARGB8888, RGBA8888
 * 16bits : ARGB1555, RGBA5551, RGB565
 * 8bits : RGB332, MONO8
 */
#define UE_CONFIG_COLOUR RGBA8888

/*
 * Enable full frame render
 *
 * Enabling this make the renderer render frame all at once. This consumes a lot
 * of RAM but makes the render faster.
 *
 * true or false
 */
#define UE_CONFIG_FULL_FRAME_RENDER false

/*
 * Enable fast clipping
 *
 * Enabling this improve the speed but triangle are not clipped, they are
 * simply not rendered if at least one of its corner is outside the frame.
 * Enable this only if you can guarantee that the object will stay inside
 * the frame at all time.
 *
 * true or false
 */
#define UE_CONFIG_FAST_CLIPPING false

#endif /* UE_CONFIG_H_ */
