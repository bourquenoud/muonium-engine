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

namespace ue {

#if UE_CONFIG_COLOUR == RGBA8888
  struct CoulourRGB
  {
    uint32_t r : 8;
    uint32_t g : 8;
    uint32_t b : 8;
    uint32_t a : 8;
  };
  union Colour
  {
    uint32_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == ARGB8888
  struct CoulourRGB
  {
    uint32_t a : 8;
    uint32_t r : 8;
    uint32_t g : 8;
    uint32_t b : 8;
  };
  union Colour
  {
    uint32_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == ARGB1555
  struct CoulourRGB
  {
    uint16_t a : 1;
    uint16_t r : 5;
    uint16_t g : 5;
    uint16_t b : 5;
  };
  union Colour
  {
    uint16_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == RGBA5551
  struct CoulourRGB
  {
    uint16_t r : 5;
    uint16_t g : 5;
    uint16_t b : 5;
    uint16_t a : 1;
  };
  union Colour
  {
    uint16_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == RGB565
  struct CoulourRGB
  {
    uint16_t r : 5;
    uint16_t g : 6;
    uint16_t b : 5;
  };
  union Colour
  {
    uint16_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == RGB332
  struct CoulourRGB
  {
    uint8_t r : 3;
    uint8_t g : 3;
    uint8_t b : 2;
  };
  union Colour
  {
    uint16_t raw;
    CoulourRGB colour;
  };
#elif UE_CONFIG_COLOUR == MONO8
  struct CoulourRGB
  {
    uint8_t l;
  };
  union Colour
  {
    uint8_t raw;
    CoulourRGB colour;
  };
#else
#error MUONIUM ENGINE : \
  No colour mode defined. See UE_CONFIG_COLOUR in ue_config.hpp
#endif

}

#endif /* COLOUR_H_ */
