#pragma once
#include <Windows.h>
#include "ImageHandler.h"
#include <utility>

namespace std
{
	inline double expApproximation( double x )
	{
		  x = 1.0 + x / 1024;
		  x *= x; x *= x; x *= x; x *= x;
		  x *= x; x *= x; x *= x; x *= x;
		  x *= x; x *= x;
		  return x;
	}

	inline void GrayScale( __in const Image & rOrigin, __out Image * pGrayImage )
	{
		UINT byteDepth = 0;
		if ( rOrigin.size == rOrigin.width * rOrigin.height * 3 )
			byteDepth = 3;
		else if ( rOrigin.size == rOrigin.width * rOrigin.height * 4 )
			byteDepth = 4;
		else if ( rOrigin.size == rOrigin.width * rOrigin.height )
			byteDepth = 1;


		_ASSERT( byteDepth == 1 || byteDepth == 3 || byteDepth == 4 );

		if ( byteDepth == 1 )
		{
			for (int i = 0; i < pGrayImage->size; i++)
			{
				pGrayImage->buffer[i] = rOrigin.buffer[i];
			}
			return;
		}

		UINT j = 0;
		for (int i = 0; i < pGrayImage->size; i++)
		{
			UINT rgbSum = 2989 * rOrigin.buffer[j + 2] + 5870 * rOrigin.buffer[j + 1] + 1140 * rOrigin.buffer[j];
			pGrayImage->buffer[i] = rgbSum / 10000;
			j += byteDepth;
		}
	}
}