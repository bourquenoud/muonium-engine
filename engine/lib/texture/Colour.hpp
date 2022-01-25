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
    inline Colour alphaBlendPost(Colour& in, Real transparency);
    inline Colour alphaBlendPre(Colour& in);
    inline void premultiplyAlpha();
    inline Colour alphaClip(Colour& in, Real threshold);
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
    inline Colour alphaBlendPost(Colour& in, Real transparency);
    inline Colour alphaBlendPre(Colour& in);
    inline void premultiplyAlpha();
    inline Colour alphaClip(Colour& in, Real threshold);
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
    inline Colour alphaBlendPost(Colour& in, Real transparency);
    inline Colour alphaBlendPre(Colour& in);
    inline void premultiplyAlpha();
    inline Colour alphaClip(Colour& in, Real threshold);
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
    inline Colour alphaBlendPost(Colour& in, Real transparency);
    inline Colour alphaBlendPre(Colour& in);
    inline void premultiplyAlpha();
    inline Colour alphaClip(Colour& in, Real threshold);
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
  };
#else
#error MUONIUM ENGINE : \
    No colour mode defined. See UE_CONFIG_COLOUR in ue_config.hpp
#endif


#ifdef UE_ALPHA_MAX
  //Alpha blend the colour with post compted values
  //TODO : make it work with all configurations
  inline Colour Colour::alphaBlendPost(Colour& in, Real transparency) //Transparency from 0 to 255
  {
    colour.a = Real::clamp((Real)colour.a * transparency, 0, UE_ALPHA_MAX);

    //Don't blend if not necessary
    if(colour.a == UE_ALPHA_MAX)
      return *this;
    else if(colour.a == 0)
      return in;

    Colour result;

    result.colour.r = ((uint16_t)colour.r * colour.a + (uint16_t)in.colour.r * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;
    result.colour.g = ((uint16_t)colour.g * colour.a + (uint16_t)in.colour.g * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;
    result.colour.b = ((uint16_t)colour.b * colour.a + (uint16_t)in.colour.b * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;

    return result;
  }

  //Alpha blend the colour with precomputed values
  //TODO : make it work with all configurations
  inline Colour Colour::alphaBlendPre(Colour& in)
  {
    //Don't blend if not necessary
    if(colour.a == UE_ALPHA_MAX)
      return *this;
    else if(colour.a == 0)
      return in;

    Colour result;

    result.colour.r = colour.r + ((uint16_t)in.colour.r * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;
    result.colour.g = colour.g + ((uint16_t)in.colour.g * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;
    result.colour.b = colour.b + ((uint16_t)in.colour.b * (UE_ALPHA_MAX - colour.a)) / UE_ALPHA_MAX;

    return result;
  }

  //TODO : make it work with all configurations
  //Precompute the alpha values to improve performance
  inline void Colour::premultiplyAlpha()
  {
    colour.r = ((uint16_t)colour.r * colour.a) / UE_ALPHA_MAX;
    colour.g = ((uint16_t)colour.g * colour.a) / UE_ALPHA_MAX;
    colour.b = ((uint16_t)colour.b * colour.a) / UE_ALPHA_MAX;
  }

  //Alpha clip the colour
  inline Colour Colour::alphaClip(Colour& in, Real threshold)
  {
    return ((colour.a) > (int)(threshold * R(255.0))) ? *this : in;
  }

#endif /* ifdef UE_ALPHA_MAX */
}

#endif /* COLOUR_H_ */
