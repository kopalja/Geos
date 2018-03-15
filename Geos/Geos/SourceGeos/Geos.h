
// Geos.h
#pragma once
#include <Windows.h>
#include "ImageHandler.h"
#include "GMM.h"
#include "Probability.h"
#include <functional>


class Location;



class _declspec(dllexport) Geos
{
public:
	Geos();
	~Geos();


	void Process( 
		__in const char *inputImagePath,
		__in Location * pForeground,
		__in Location * pBackground,
		__in int sharpType,
		__in int timeOptimalization,
		__in int smoothness,
		__in int colorRepresentation
		);

	bool ** m_ppResult;
	int m_width;
	int m_height;

private:
	SegmentationType m_SegmentationType;
	int m_TimeOptimalization;
	int m_Smoothness;
	bool m_RgbType;




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
		__in const Image & rOrigin
		);

	void SetSegmentationParameters(
		__in int sharpType,
		__in int timeOptimalization,
		__in int smoothness,
		__in int colorRepresentation
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



/*
// Without Interactive help
extern "C"  __declspec(dllexport)  void _stdcall  SegmentationProcess( 
	const char * imagePath, 
	int segmentationType, 
	int timeOptimalization,
	int boundSmoothness,
	int colorRepresentation,
	INT32 * data 
	)
{
		Geos g;
//		g.Process( imagePath );
		for (int y = 0; y < g.m_height; y++)
		{
			for (int x = 0; x < g.m_width; x++)
			{
				if ( g.m_ppResult[x][y] )
					data[y * g.m_width + x] = 1;
				else 
					data[y * g.m_width + x] = -1;
			}
		}
}

*/


// With help
extern "C"  __declspec(dllexport)  void _stdcall  SegmentationProcess( 
	const char * imagePath, 
	int segmentationType, 
	int timeOptimalization,
	int boundSmoothness,
	int colorRepresentation, 
	INT32 * data, 
	INT32 * foregroundX, 
	INT32 * foregroundY, 
	INT32 * backgroundX, 
	INT32 * backgroundY, 
	int foregroundSize,
	int backgroundSize
	)
{
	Geos g;
	if (foregroundSize == 0) // without help
	{
		g.Process( imagePath, nullptr, nullptr, segmentationType, timeOptimalization, boundSmoothness, colorRepresentation );
	}
	else // without help
	{
		Location * foreground = new Location[foregroundSize];
		Location * background = new Location[backgroundSize];

		for (int i = 0; i < foregroundSize; i++)
		{
			foreground[i].x = foregroundX[i];
			foreground[i].y = foregroundY[i];
		}
		for (int i = 0; i < backgroundSize; i++)
		{
			background[i].x = backgroundX[i];
			background[i].y = backgroundY[i];
		}
		g.Process( imagePath, foreground, background, segmentationType, timeOptimalization, boundSmoothness, colorRepresentation );
	}

	for (int y = 0; y < g.m_height; y++)
	{
		for (int x = 0; x < g.m_width; x++)
		{
			if ( g.m_ppResult[x][y] )
				data[y * g.m_width + x] = 1;
			else 
				data[y * g.m_width + x] = -1;
		}
	}
}



