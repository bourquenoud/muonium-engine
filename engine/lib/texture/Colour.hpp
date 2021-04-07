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

#ifdef UE_CONFIG_COLOUR_RGBA
  struct CoulourRGB
  {
          uint32_t r : 8;
          uint32_t g : 8;
          uint32_t b : 8;
          uint32_t a : 8;
  };
#endif

  union Colour
  {
          uint32_t raw;
          CoulourRGB colour;
  };

}

#endif /* COLOUR_H_ */
