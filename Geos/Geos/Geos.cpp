// This is the main DLL file.

#include "stdafx.h"

#include "Geos.h"

#include <iostream>
#include <ctime>

using namespace std;

const double sqrt2 = 1.41421356237;

Geos::Geos()
{

}

Geos::~Geos()
{

}



void Geos::Process( __in const char *inputPath, __in const char *outputPath )
{
	HRESULT hr;
	ImageHandler imageHandler( inputPath, outputPath, &hr );

	if ( SUCCEEDED( hr ) )
	{
		/* Aloc origin image */
		Image *pOrigin = new Image(
			imageHandler.InputImageWidth(), 
			imageHandler.InputImageHeight(), 
			imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth(),
			new BYTE[imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth()] 
		);
		hr = imageHandler.Create( pOrigin );
		if ( SUCCEEDED( hr ) )
		{
			/* Aloc gray image */
			Image *pGrayImage = new Image(
				imageHandler.InputImageWidth(), 
				imageHandler.InputImageHeight(), 
				imageHandler.InputImageWidth() * imageHandler.InputImageHeight(),
				new BYTE[imageHandler.InputImageWidth() * imageHandler.InputImageHeight()] 
			);
			GrayScale( pOrigin, pGrayImage );

			/* Aloc distance map */
			double **ppGeodesicDistance = new double*[imageHandler.InputImageWidth()];
			for (int i = 0; i < imageHandler.InputImageWidth(); i++)
				ppGeodesicDistance[i] = new double[imageHandler.InputImageHeight()];

			for (int y = 0; y < imageHandler.InputImageHeight(); y++)
				for (int x = 0; x < imageHandler.InputImageWidth(); x++)
					ppGeodesicDistance[x][y] = UINT_MAX / 8;

			for (int y = 200; y < imageHandler.InputImageHeight(); y++)
			{
				for (int x = 150; x < 300; x++)
				{
					//ppGeodesicDistance[x][y] = 0;
				}
			}
			ppGeodesicDistance[0][0] = 0;
			double alfa = 0.0;

			std::clock_t clock;
			int start = std::clock();
			int duration;

			GeodesicDistance( alfa, pGrayImage, ppGeodesicDistance );
			GeodesicDistance( alfa, pGrayImage, ppGeodesicDistance );
			GeodesicDistance( alfa, pGrayImage, ppGeodesicDistance );
			GeodesicDistance( alfa, pGrayImage, ppGeodesicDistance );

			DistancesToImage( ppGeodesicDistance, pGrayImage );

			cout <<  std::clock() - start << endl;




			imageHandler.Save( pGrayImage );

			delete pGrayImage;
		}
		delete pOrigin;
	}

}


void Geos::GrayScale( __in const Image *pOrigin, __out Image *pGrayImage )
{
	UINT byteDepth = 0;
	if ( pOrigin->size == pOrigin->width * pOrigin->height * 3 )
		byteDepth = 3;
	else if ( pOrigin->size == pOrigin->width * pOrigin->height * 4 )
		byteDepth = 4;
	_ASSERT( byteDepth == 3 || byteDepth == 4 );

	UINT j = 0;
	for (int i = 0; i < pGrayImage->size; i++)
	{
		UINT rgbSum = 2989 * pOrigin->buffer[j + 2] + 5870 * pOrigin->buffer[j + 1] + 1140 * pOrigin->buffer[j];
		pGrayImage->buffer[i] = rgbSum / 10000;
		j += byteDepth;
	}
}


void Geos::GeodesicDistance( __in double alfa, __in const Image *pGrayImage, __inout double **ppGeodesicDistance )
{
	int dxa = - pGrayImage->width - 1;
	int dxb = - pGrayImage->width; 
	int dxc = - pGrayImage->width + 1;
	int dxd = - 1; 
	int index = pGrayImage->width;
	double da, db, dc, dd;


	for (int y = 1; y < pGrayImage->height - 1; y++)
	{
		index++;
		for (int x = 1; x < pGrayImage->width - 1; x++)
		{
			GrayBaseNeighborDistances( pGrayImage->buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );

			double d1 = sqrt2 + alfa * da + ppGeodesicDistance[x - 1][y - 1];
			double d2 = 1.0 + alfa * db + ppGeodesicDistance[x - 0][y - 1];
			double d3 = sqrt2 + alfa * dc + ppGeodesicDistance[x + 1][y - 1];
			double d4 = 1.0 + alfa * dd + ppGeodesicDistance[x - 1][y - 0];

			if ( ppGeodesicDistance[x][y] > d1 )
				ppGeodesicDistance[x][y] = d1;
			if ( ppGeodesicDistance[x][y] > d2 )
				ppGeodesicDistance[x][y] = d2;
			if ( ppGeodesicDistance[x][y] > d3 )
				ppGeodesicDistance[x][y] = d3;
			if ( ppGeodesicDistance[x][y] > d4 )
				ppGeodesicDistance[x][y] = d4;
			index++;
		}
		index++;
	}

	dxa = 1;
	dxb = pGrayImage->width - 1; 
	dxc = pGrayImage->width;
	dxd = pGrayImage->width + 1; 

	for (int y = pGrayImage->height - 2; y >= 1; y--)
	{
		index--;
		for (int x = pGrayImage->width - 2; x >= 1; x--)
		{
			index--;
			GrayBaseNeighborDistances( pGrayImage->buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );
			double d1 = 1.0 + alfa * da + ppGeodesicDistance[x + 1][y - 0];
			double d2 = sqrt2 + alfa * db + ppGeodesicDistance[x - 1][y + 1];
			double d3 = 1.0 + alfa * dc + ppGeodesicDistance[x + 0][y + 1];
			double d4 = sqrt2 + alfa * dd + ppGeodesicDistance[x + 1][y + 1];

			if ( ppGeodesicDistance[x][y] > d1 )
				ppGeodesicDistance[x][y] = d1;
			if ( ppGeodesicDistance[x][y] > d2 )
				ppGeodesicDistance[x][y] = d2;
			if ( ppGeodesicDistance[x][y] > d3 )
				ppGeodesicDistance[x][y] = d3;
			if ( ppGeodesicDistance[x][y] > d4 )
				ppGeodesicDistance[x][y] = d4;
		}
		index--;
	}
}


inline void Geos::GrayBaseNeighborDistances( __in const BYTE *pBuffer, __in UINT index, __in int dxa, __in int dxb, __in int dxc, __in int dxd, __out double *da, __out double *db, __out double *dc, __out double *dd )
{
	int shift = sizeof(int) * CHAR_BIT - 1;
	int i, mask;
	i = pBuffer[index] - pBuffer[index + dxa];
	mask = i >> shift;
	*da = (i + mask) ^ mask;

	i = pBuffer[index] - pBuffer[index + dxb];
	mask = i >> shift;
	*db = (i + mask) ^ mask;

	i = pBuffer[index] - pBuffer[index + dxc];
	mask = i >> shift;
	*dc = (i + mask) ^ mask;

	i = pBuffer[index] - pBuffer[index + dxd];
	mask = i >> shift;
	*dd = (i + mask) ^ mask;
}



void Geos::DistancesToImage( __in double **ppGeodesicDistance, __out Image *pGrayImage )
{
	double maxD = 0;
	for (int y = 1; y < pGrayImage->height - 1; y++)
		for (int x = 1; x < pGrayImage->width - 1; x++)
			maxD = max( maxD, ppGeodesicDistance[x][y] );

	UINT i = 0;
	for (int y = 0; y < pGrayImage->height; y++)
		for (int x = 0; x < pGrayImage->width; x++)
		{
			pGrayImage->buffer[i] = 255.0 * ( ppGeodesicDistance[x][y] / maxD );
			i++;
		}
}