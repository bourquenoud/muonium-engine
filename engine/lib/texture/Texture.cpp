/*
 * Texture.cpp
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#include "Texture.hpp"

namespace ue
{
#if UE_CONFIG_COLOUR == ARGB8888 \
 or UE_CONFIG_COLOUR == RGBA8888 \
 or UE_CONFIG_COLOUR == ARGB1555 \
 or UE_CONFIG_COLOUR == RGBA5551
  //Premultiply all pixel with the alpha value
  void Texture::premultiplyAlpha(Real transparency)
  {
    for(uint32_t i = 0; i < width*height; i++)
    {
      pixel[i].colour.a = (Real)pixel[i].colour.a * transparency;
      pixel[i].premultiplyAlpha();
    }
  }
#endif

  void Texture::setDimensions(uint32_t _width, uint32_t _height)
  {
	  width = _width;
	  height = _height;

	  preprocessedWidth = (Real)(width);
	  preprocessedHeight = (Real)(height);
  }
}
