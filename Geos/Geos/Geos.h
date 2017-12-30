// Geos.h
#include <Windows.h>
#include "ImageHandler.h"
#include "SymmetricalFilter.h"
#include "GMM.h"
#include <functional>

class Location
{
public:
	int x;
	int y;
};

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
		__in const Image & pOrigin,
		__out Image *grayImage
		);

	void GeodesicDistance(
		__in double alfa,
		__in const Image *pGrayImage,
		__inout double **ppGeodesicDistance
		);



	void DistancesToImage(
		__in double **ppGeodesicDistance,
		__out Image *pGrayImage
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
