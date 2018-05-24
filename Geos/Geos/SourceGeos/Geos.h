
// Geos.h
#pragma once
#include <Windows.h>
#include "ImageHandler.h"
#include "GMM.h"
#include "Probability.h"

#include <functional>



class Location;



class Geos
{
public:
	Geos();
	~Geos();


	void Process( 
		__in const char *inputImagePath,
		__in const vector<Location> & rForeGround,
		__in const vector<Location> & rBackGround,
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

	Image * m_pGrayImage;





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
		__in const Image & rOrigin,
		__in const vector<Location> & rForeGround,
		__in const vector<Location> & rBackGround
		);

	void SetSegmentationParameters(
		__in int sharpType,
		__in int timeOptimalization,
		__in int smoothness,
		__in int colorRepresentation
		);

	void MinimizeEnegry(
		__in const Image & rOrigin,
		__in double ** ppProbability
		);

	double CountEnegry(
		__in const Image & rGrayImage,
		__in double ** ppProbability,
		__in bool ** ppLabeling
		);

	/* Copy labeling into result array */
	void CopyLabeling(
		__in int width,
		__in int height,
		__in bool ** ppInputLabeling
	)
	{
		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				m_ppResult[x][y] = ppInputLabeling[x][y];
			}
		}
	}






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
	Entry point of the library.
	All parameters are passed in.
*/



extern "C"
{
	__declspec(dllexport) void  SegmentationProcess(
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
		vector<Location> * pForeGround = new vector<Location>();
		vector<Location> * pBackGround = new vector<Location>();

		/* user helped interactively */
		if (foregroundSize > 1 && backgroundSize > 1)
		{
			for (int i = 0; i < foregroundSize; i++)
			{
				pForeGround->push_back(Location(foregroundX[i], foregroundY[i]));
			}
			for (int i = 0; i < backgroundSize; i++)
			{
				pBackGround->push_back(Location(backgroundX[i], backgroundY[i]));
			}
		}

		/* Segmentation method */
		g.Process(imagePath, const_cast<vector<Location>&>(*pForeGround), const_cast<vector<Location>&>(*pBackGround), segmentationType, timeOptimalization, boundSmoothness, colorRepresentation);
		delete pForeGround;
		delete pBackGround;

		/* Copying result into one dimensional array */
		for (int y = 0; y < g.m_height; y++)
		{
			for (int x = 0; x < g.m_width; x++)
			{
				if (g.m_ppResult[x][y])
					data[y * g.m_width + x] = 1;
				else
					data[y * g.m_width + x] = -1;
			}
		}
	}
}



