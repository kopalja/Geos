// Geos.h
#include <Windows.h>
#include "ImageHandler.h"



#pragma once
class _declspec(dllexport) Geos
{
public:
	Geos();
	~Geos();

	void Process( 
		__in const char *inputPath,
		__in const char *outputPath 
		);

private:
	void GrayScale(
		__in const Image *origin,
		__out Image *grayImage
		);
};
