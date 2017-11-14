// This is the main DLL file.

#include "stdafx.h"

#include "Geos.h"


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

