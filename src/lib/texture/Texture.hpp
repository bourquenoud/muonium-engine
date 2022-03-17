/*
 * Texture.h
 *
 *  Created on: 6 Apr 2021
 *      Author: mathieu
 */

#ifndef TEXTURE2_H_
#define TEXTURE2_H_

#include <cstdint>
#include <cstdio>

#include "../linal.hpp"
#include "Colour.hpp"
#include "../arith.hpp"

namespace ue
{

	class Texture
	{
	public:
		Colour *pixel;

		inline Colour getPixelAt(const Vector2 &pos) const
		{
			// FIXME: no flooring create errors with FIXED32
			uint32_t x = pos.x * (preprocessedWidth - R(1.0));
			uint32_t y = (R(1.0) - pos.y) * (preprocessedHeight - R(1.0));

			return pixel[(uint32_t)(x + (width * y))];
		}

		inline Colour getPixelAt_old(const Vector2 &pos) const
		{
			// TODO : make it so we don't have to floor the value (Something is wrong somewhere)
			uint32_t x = (uint32_t)Real::floor(pos.x * (Real)(width - 1));
			uint32_t y = (uint32_t)Real::floor((R(1.0) - pos.y) * (Real)(height - 1));

			return pixel[x + (width * y)];
		}

		void premultiplyAlpha(Real);
		void setDimensions(uint32_t _width, uint32_t _height);

	private:
		uint32_t width;
		uint32_t height;
		Real preprocessedWidth;
		Real preprocessedHeight;
	};

}

#endif /* TEXTURE2_H_ */
