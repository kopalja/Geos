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

	void GeodesicDistance(
		__in double alfa,
		__in const Image *pGrayImage,
		__inout double **ppGeodesicDistance
		);

	inline void GrayBaseNeighborDistances(
		__in const BYTE *pBuffer,
		__in UINT index,
		__in int dxa,
		__in int dxb,
		__in int dxc,
		__in int dxd,
		__out double *da,
		__out double *db,
		__out double *dc,
		__out double *dd
		);


	void DistancesToImage(
		__in double **ppGeodesicDistance,
		__out Image *pGrayImage
		);

};
