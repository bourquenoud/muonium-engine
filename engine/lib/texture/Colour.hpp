/*
 * Colour.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef COLOUR_H_
#define COLOUR_H_

#include <cstdint>

#include "../../ue_config.hpp"
#include "../arith.hpp"

namespace ue {

#if UE_CONFIG_COLOUR == RGBA8888
#define UE_ALPHA_MAX 255
  struct ColourRGB
  {
    uint32_t r : 8;
    uint32_t g : 8;
    uint32_t b : 8;
    uint32_t a : 8;
  };
  union Colour
  {
    uint32_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == ARGB8888
#define UE_ALPHA_MAX 255
  struct ColourRGB
  {
    uint32_t a : 8;
    uint32_t r : 8;
    uint32_t g : 8;
    uint32_t b : 8;
  };
  union Colour
  {
    uint32_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == ARGB1555
#define UE_ALPHA_MAX 1
  struct ColourRGB
  {
    uint16_t a : 1;
    uint16_t r : 5;
    uint16_t g : 5;
    uint16_t b : 5;
  };
  union Colour
  {
    uint16_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == RGBA5551
#define UE_ALPHA_MAX 1
  struct ColourRGB
  {
    uint16_t r : 5;
    uint16_t g : 5;
    uint16_t b : 5;
    uint16_t a : 1;
  };
  union Colour
  {
    uint16_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == RGB565
  struct ColourRGB
  {
    uint16_t r : 5;
    uint16_t g : 6;
    uint16_t b : 5;
  };
  union Colour
  {
    uint16_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == RGB332
  struct ColourRGB
  {
    uint8_t r : 3;
    uint8_t g : 3;
    uint8_t b : 2;
  };
  union Colour
  {
    uint16_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#elif UE_CONFIG_COLOUR == MONO8
  struct ColourRGB
  {
    uint8_t l;
  };
  union Colour
  {
    uint8_t raw;
    ColourRGB colour;
    inline Colour over(Colour& in, Real transparency);
  };
#else
#error MUONIUM ENGINE : \
    No colour mode defined. See UE_CONFIG_COLOUR in ue_config.hpp
#endif

  //TODO : make it work with all configurations
  inline Colour Colour::over(Colour& in, Real transparency)
  {
    Colour result;
    colour.a = Real::clamp((Real)colour.a * transparency, 0, UE_ALPHA_MAX);

    //Don't blend if not necessary
    if(colour.a == UE_ALPHA_MAX)
      return *this;
    else if(colour.a == 0)
      return in;

    result.colour.r = ((uint16_t)colour.r * colour.a + (uint16_t)in.colour.r * (UE_ALPHA_MAX - colour.a)) / 255;
    result.colour.g = ((uint16_t)colour.g * colour.a + (uint16_t)in.colour.g * (UE_ALPHA_MAX - colour.a)) / 255;
    result.colour.b = ((uint16_t)colour.b * colour.a + (uint16_t)in.colour.b * (UE_ALPHA_MAX - colour.a)) / 255;

    return result;
  }
}

#endif /* COLOUR_H_ */
