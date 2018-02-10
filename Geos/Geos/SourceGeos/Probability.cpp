#include "Probability.h"	

#include <ctime>

void Probability::GetProbabitily( __in SegmentationType segmentationType, __in const Image & rOrigin, __out double ** ppProbability, __in const Location * pForeground, __in const Location * pBackground )
{
	switch ( segmentationType )
	{
	case Interactive:
		{
			InteractiveProbability( rOrigin, pForeground, pBackground, ppProbability );
			break;
		}
	case Sharpness:
		{
			SharpnessProbability( rOrigin, ppProbability );
			break;
		}
	case Color:
		{
			GMM g;
			g.AutomaticProbability( rOrigin, ppProbability );
			break;
		}
	case Salience:
		{
			SalienceProbability( rOrigin, ppProbability );
			break;
		}
	}
}

void Probability::InteractiveProbability( __in const Image & rOrigin, __in const Location * pForeground, __in const Location * pBackground, __out double ** ppProbability )
{
	SegmentInteractive( rOrigin, pForeground, ppProbability );

	double ** ppBackgroundProbability = new double*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
		ppBackgroundProbability[i] = new double[rOrigin.height];

	SegmentInteractive( rOrigin, pForeground, ppBackgroundProbability );

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			ppProbability[x][y] = log( ppProbability[x][y] ) - log( ppBackgroundProbability[x][y] );
			ppProbability[x][y] = 1.0 / ( 1.0 + exp( - ppProbability[x][y] / 10.0 ) );
		}
	}

	for (int i = 0; i < rOrigin.width; i++)
		delete ppBackgroundProbability[i];
	delete ppBackgroundProbability;
}


void Probability::SharpnessProbability( __in const Image & rOrigin, __out double ** ppProbability )
{
	int xDivisible = rOrigin.width, yDivisible = rOrigin.height;

	if (rOrigin.width % 8 != 0)
		xDivisible = rOrigin.width + ( 8 - ( rOrigin.width % 8 ) );
	if (rOrigin.height % 8 != 0)
		yDivisible = rOrigin.height + ( 8 - ( rOrigin.height % 8 ) );



	

	S3 s3;
	Image *pGrayImage = new Image( xDivisible, yDivisible, xDivisible * yDivisible , new BYTE[xDivisible * yDivisible] );
	Image *pResultImage = new Image( xDivisible / 4, yDivisible / 4, ( xDivisible * yDivisible ) / 16, new BYTE[( xDivisible * yDivisible ) / 16] );
	s3.GrayScale( rOrigin, pGrayImage );
	s3.GrayToResult( ResultType::S3Image, 2, false, const_cast<Image &>( *pGrayImage ), pResultImage );

	for (int i = 1; i < pResultImage->height; i++)
	{
		pResultImage->buffer[( i * pResultImage->width ) - 1] = 0;
		pResultImage->buffer[( i * pResultImage->width ) - 2] = 0;
	}

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			ppProbability[x][y] = (double)pResultImage->buffer[ ( y >> 2 ) * pResultImage->width + ( x >> 2 ) ] / 255.0;
		}
	}

	/* Fill probability inside sharp area */
	double ** ppDistance = new double*[pResultImage->width];
	for (int i = 0; i < pResultImage->width; i++)
	{
		ppDistance[i] = new double[pResultImage->height];
	}
	
	SymmetricalFilter symmetricalFilter( 1.0 );
	InitDistance( pResultImage->width, pResultImage->height, ppDistance );

	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );
	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );
	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );
	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );
	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );
	symmetricalFilter.CountUnSignedDistance( pResultImage, ppDistance, false );

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			if ( ppDistance[x >> 2][y >> 2] > 80000 )
			{
				ppProbability[x][y] = max( ppProbability[x][y], ( ppDistance[x >> 2][y >> 2] - 80000.0 ) / 150000.0 ); 
				if ( ppProbability[x][y] > 1.0 )
					ppProbability[x][y] = 1.0;
			}
		}
	}

	for (int i = 0; i < pResultImage->width; i++)
	{
		delete ppDistance[i];
	}
	delete ppDistance;
	delete pGrayImage;
	delete pResultImage;
}

void Probability::SalienceProbability( __in const Image & rOrigin, __out double ** ppProbability )
{
	BYTE red;
	BYTE green;
	BYTE blue;
	Image * pBlurImage = new Image( rOrigin.width, rOrigin.height, rOrigin.size, new BYTE[ rOrigin.size ] );
	GaussianBlur gaussianBlur;
	gaussianBlur.Process( 0.0, rOrigin, pBlurImage );
	AverageColor( rOrigin, &red, &green, &blue );

	int i = 0;
	for (int y = 0; y < pBlurImage->height; y++)
	{
		for (int x = 0; x < pBlurImage->width; x++)
		{
			int dif1 = pBlurImage->buffer[i + 2] - red;
			int dif2 = pBlurImage->buffer[i + 1] - green;
			int dif3 = pBlurImage->buffer[i + 0] - blue;
			double color = sqrt( dif1 * dif1 + dif2 * dif2 + dif3 * dif3 );
			ppProbability[x][y] = color / 255.0;
			if (ppProbability[x][y] < 0.5)
				ppProbability[x][y] = 0;
			else
				ppProbability[x][y] = 1;
			//ppProbability[x][y] *= ppProbability[x][y] * ppProbability[x][y];
			//ppProbability[x][y] *= 2.0;
			
			i += 3;
		}
	}
}

void Probability::AverageColor( __in const Image & rOrigin, __out BYTE * red, __out BYTE * green, __out BYTE * blue )
{
	long lRed = 0;
	long lGreen = 0;
	long lBlue = 0;
	for (int i = 0; i < rOrigin.size; i += 3)
	{
		lRed += rOrigin.buffer[i + 2];
		lGreen += rOrigin.buffer[i + 1];
		lBlue += rOrigin.buffer[i];
	}
	UINT numberOfPixels = rOrigin.width * rOrigin.height;
	*red = lRed / numberOfPixels;
	*green = lGreen / numberOfPixels;
	*blue = lBlue / numberOfPixels;
}

void Probability::InitDistance( __in int width, __in int height, __out double ** ppDistance )
{
	for (int i = 0; i < width; i++)
	{
		ppDistance[i][0] = 0;
	}
	for (int y = 1; y < height; y++)
	{
		ppDistance[0][y] = 0;
		for (int x = 1; x < width - 1; x++)
		{
			ppDistance[x][y] = MAXINT32;
		}
		ppDistance[width - 1][y] = 0;
		ppDistance[width - 2][y] = 0;
	}
}


void Probability::SegmentInteractive( __in const Image & rOrigin, __in const Location * pSelectedPixel, __out double ** ppProbability )
{

}
