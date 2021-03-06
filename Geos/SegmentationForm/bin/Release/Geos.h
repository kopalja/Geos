
// Geos.h
#pragma once
#include <Windows.h>
#include "ImageHandler.h"
#include "GMM.h"
#include "Probability.h"
#include <functional>

class Location;

extern "C" 
{
	extern __declspec(dllexport) void foo();
}


class _declspec(dllexport) Geos
{
public:
	Geos();
	~Geos();

	void Process( 
		__in const char *inputOriginPath,
		__in const char *inputInteractivePath,
		__in const char *outputPath, 
		__in int erosion, 
		__in int diletation
		);

private:
	void GrayScale(
		__in const Image & rOrigin,
		__out Image *pGrayImage
		);


	void GeodesicDistance(
		__in double alfa,
		__in const Image *pGrayImage,
		__inout double **ppGeodesicDistance
		);


	void GetForegraundLocations( Image * pImage,  bool ** output, bool type )
	{
		int i = 0;
		for (int y = 0; y < pImage->height; y++)
		{
			for (int x = 0; x < pImage->width; x++)
			{
				if (type == true)
				{
					if ( pImage->buffer[i] < 50 && pImage->buffer[i + 1] > 230 && pImage->buffer[i + 2] < 50 )
					{
						output[x][y] = true;
					}
				}
				else
				{
					if ( pImage->buffer[i] < 50 && pImage->buffer[i + 1] < 50 && pImage->buffer[i + 2] > 230 )
					{
						output[x][y] = true;
					}
				}
				i += 3;
			}
		}
	}

	void ImageSegmentation(
		__in const Image & pInteractiveImage,
		__in int erosion,
		__in int diletation,
		__inout Image & pOrigin
		);

	void MinimizeEnegry(
		__in const Image & rOrigin,
		__in double ** ppProbability,
		__out bool ** ppLabeling
		);

	double CountEnegry(
		__in const Image & rGrayImage,
		__in double ** ppProbability,
		__in bool ** ppLabeling
		);






	//-----------------------------------------------------------------

	void SetPixelProbability(
		__in const Image & pInteractiveImage,
		__in const Image * pGrayImage,
		__in const Image & pOrigin,
		__out double ** ppProbability
		);


	void SetLocations(
		__in const Image & pInteractiveImage,
		__in std::function<bool ( BYTE, BYTE, BYTE )> condition,
		__out bool ** ppLocations
		);

	void SetAreaProbability(
		__in const Image * pGrayImage,
		__in const Image & pOrigin,
		__in bool ** ppLocations,
		__out double ** ppProbability
		);



};


void foo()
{
	cout << "aass" << endl;
	Geos g;
	g.Process( "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\tiger.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\tiger.jpg", "C:\\Users\\User\\Documents\\GitHub\\Geos\\samples\\results\\out16.jpg", 0, 0 );
}
