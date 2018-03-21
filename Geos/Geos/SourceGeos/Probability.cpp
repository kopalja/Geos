#include "Probability.h"
#include <ctime>

void Probability::GetProbabitily( __in SegmentationType segmentationType, __in const Image & rOrigin, __in const Image & rGrayImage, __in const vector<Location> & rForeground, __in const vector<Location> & rBackground, __out double ** ppProbability )
{
	switch ( segmentationType )
	{
	case Sharpness:
		{
			SharpnessProbability( rOrigin, ppProbability );
			break;
		}
	case SharpAndColor:
		{
			SharpnessProbability( rOrigin, ppProbability, true );
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

	/* in case of interactive probability */
	if ( rForeground.size() > 0 && rBackground.size() > 0 )
		InteractiveProbability( rOrigin, rGrayImage, rForeground, rBackground, ppProbability );
}



void Probability::InteractiveProbability( __in const Image & rOrigin, __in const Image & rGrayImage, __in const vector<Location> & rForeGround, __in const vector<Location> & rBackGround, __inout double ** ppProbability )
{
	double MAX_DISTANCE = 700.0;
	double ** ppForeGroundDistance = new double*[rOrigin.width];
	double ** ppBackGroundDistance = new double*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
	{
		ppForeGroundDistance[i] = new double[rOrigin.height];
		ppBackGroundDistance[i] = new double[rOrigin.height];
	}

	for (int y = 0; y < rOrigin.height; y++)
		for (int x = 0; x < rOrigin.width; x++)
		{
			ppForeGroundDistance[x][y] = MAX_DISTANCE;
			ppBackGroundDistance[x][y] = MAX_DISTANCE;
		}

	for (int i = 0; i < rForeGround.size(); i++)
	{
		ppForeGroundDistance[rForeGround.at( i ).x][rForeGround.at( i ).y] = 0; 
	}
	for (int i = 0; i < rBackGround.size(); i++)
	{
		ppBackGroundDistance[rBackGround.at( i ).x][rBackGround.at( i ).y] = 0; 
	}

	SymmetricalFilter s( 2.0, rGrayImage );
	s.CountUnSignedDistance( ppForeGroundDistance );
	s.CountUnSignedDistance( ppForeGroundDistance );
	s.CountUnSignedDistance( ppForeGroundDistance );

	s.CountUnSignedDistance( ppBackGroundDistance );
	s.CountUnSignedDistance( ppBackGroundDistance );
	s.CountUnSignedDistance( ppBackGroundDistance );


	double ** ppGmmProbability = new double*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
	{
		ppGmmProbability[i] = new double[rOrigin.height];
	}

	GMM g;
	g.InteractiveProbability( rOrigin, rForeGround, rBackGround, ppGmmProbability );


	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			double temp = 1.0 - ( MAX_DISTANCE + ppForeGroundDistance[x][y] - ppBackGroundDistance[x][y] ) / ( MAX_DISTANCE * 2.0 );

			ppProbability[x][y] = ( ppProbability[x][y] + ppGmmProbability[x][y] + temp ) / 3;
		}
	}


	for (int i = 0; i < rOrigin.width; i++)
	{
		delete[] ppGmmProbability[i];
	}
	delete[] ppGmmProbability;
}






void Probability::SharpnessProbability( __in const Image & rOrigin, __out double ** ppProbability, __in bool UseModifySharpness2 )
{
	clock_t time = clock();
	cout << clock() - time << endl;
	time = clock();
	int xDivisible = rOrigin.width, yDivisible = rOrigin.height;

	if (rOrigin.width % 8 != 0)
		xDivisible = rOrigin.width + ( 8 - ( rOrigin.width % 8 ) );
	if (rOrigin.height % 8 != 0)
		yDivisible = rOrigin.height + ( 8 - ( rOrigin.height % 8 ) );

	S3 s3;
	Image *pGrayImage = new Image( xDivisible, yDivisible, xDivisible * yDivisible , new BYTE[xDivisible * yDivisible] );
	Image *pS3Image = new Image( xDivisible / 4, yDivisible / 4, ( xDivisible * yDivisible ) / 16, new BYTE[( xDivisible * yDivisible ) / 16] );
	s3.GrayScale( rOrigin, pGrayImage );
	s3.GrayToResult( ResultType::S3Image, 2, false, const_cast<Image &>( *pGrayImage ), pS3Image );

	cout << "s3 " << clock() - time << endl;
	time = clock();

	for (int i = 1; i < pS3Image->height; i++)
	{
		pS3Image->buffer[( i * pS3Image->width ) - 1] = 0;
		pS3Image->buffer[( i * pS3Image->width ) - 2] = 0;
	}

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			ppProbability[x][y] = (double)pS3Image->buffer[ ( y >> 2 ) * pS3Image->width + ( x >> 2 ) ] / 255.0;
		}
	}
	ModifySharpness1( const_cast<Image &>( *pS3Image ), rOrigin, ppProbability );
	cout << "m1 " << clock() - time << endl;
	time = clock();
	if ( UseModifySharpness2 )
	{
		ModifySharpness2( const_cast<Image &>( *pS3Image ), rOrigin, ppProbability );
		cout << "m2 " << clock() - time << endl;
		time = clock();
	}
	delete pGrayImage;
	delete pS3Image;
}


void Probability::ModifySharpness1( __in const Image & rS3Image, __in const Image & rOrigin, __inout double ** ppProbability )
{
	/* Fill probability inside sharp area */
	double ** ppDistance = new double*[rS3Image.width];
	for (int i = 0; i < rS3Image.width; i++)
	{
		ppDistance[i] = new double[rS3Image.height];
	}
	
	SymmetricalFilter symmetricalFilter( 1.0, rS3Image );
	InitDistance( rS3Image.width, rS3Image.height, ppDistance );

	for (int i = 0; i < 6; i++)
		symmetricalFilter.CountUnSignedDistance( ppDistance, false );

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			if ( ppDistance[x >> 2][y >> 2] > 100000 )
			{
				ppProbability[x][y] = max( ppProbability[x][y], ( ppDistance[x >> 2][y >> 2] - 100000.0 ) / 700000.0 ); 
				if ( ppProbability[x][y] > 1.0 )
					ppProbability[x][y] = 1.0;
			}
		}
	}

	for (int i = 0; i < rS3Image.width; i++)
	{
		delete ppDistance[i];
	}
	delete ppDistance;
}

void Probability::ModifySharpness2( __in const Image & rS3Image, __in const Image & rOrigin, __inout double ** ppProbability )
{
	int numberOfForegroundSamples = 0;
	int numberOfBackgroundSamples = 0;
	double ** ppGmmProbability = new double*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
	{
		ppGmmProbability[i] = new double[rOrigin.height];
	}

	vector<Location> * pForeGround = new vector<Location>();
	vector<Location> * pBackGround = new vector<Location>();

	SetSamples( rOrigin.width, rOrigin.height, ppProbability, pForeGround, pBackGround );


	GMM g;
	
	g.InteractiveProbability( rOrigin, const_cast<vector<Location>&>( *pForeGround ), const_cast<vector<Location>&>( *pBackGround ), ppGmmProbability );

	Image *pGrayImage = new Image( rOrigin.width, rOrigin.height, rOrigin.width * rOrigin.height, new BYTE[rOrigin.width * rOrigin.height] );
	GrayScale( rOrigin, pGrayImage );
	SymmetricalFilter s( 2.0, const_cast<Image &>( *pGrayImage ), const_cast<const double **>( ppProbability ) );

	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			double temp = s.m_ppSignedDistance[x][y] + 200;
			temp = ( 400 - temp ) / 400.0;

			ppProbability[x][y] = ( ppProbability[x][y] + ppGmmProbability[x][y] + temp ) / 3;
		}
	}

	for (int i = 0; i < rOrigin.width; i++)
	{
		delete[] ppGmmProbability[i];
	}
	delete pForeGround;
	delete pBackGround;
	delete[] ppGmmProbability;
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

