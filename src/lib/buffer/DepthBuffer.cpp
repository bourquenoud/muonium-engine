/*
 * DepthBuffer.cpp
 *
 *  Created on: 8 Apr 2021
 *      Author: mathieu
 */

#include "DepthBuffer.hpp"

namespace ue
{
	DepthBuffer::DepthBuffer()
	{
		width = 0;
		height = 0;
		buffer = NULL;
	}

	DepthBuffer::DepthBuffer(uint16_t width_, uint16_t height_, Real *buffer_)
	{
		width = width_;
		height = height_;
		buffer = buffer_;
	}

	void DepthBuffer::clear()
	{
		for (uint32_t i = 0; i < width * height; i++)
		{
			buffer[i] = UE_REAL_MIN;
		}
	}
}
