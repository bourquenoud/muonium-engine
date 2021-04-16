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
   * The fixed point is 16.16 signed. Range is 32'767.99998 to -32'768.00000
   * step is 1/65536 = 15.25878906e-6
   *
   * Mode available
   * FLOAT
   * FIXED32
   */
  #define UE_CONFIG_ARITHMETIC FLOAT

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
  #define UE_CONFIG_ENABLE_NORMAL true

  /*
   * Define the colour mode of the buffer.
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
   * of RAM but makes the render somewhat faster.
   *
   * true or false
   */
  #define UE_CONFIG_FULL_FRAME_RENDER true

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
  #define UE_CONFIG_FAST_CLIPPING true

  #endif /* UE_CONFIG_H_ */
