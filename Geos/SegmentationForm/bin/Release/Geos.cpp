// This is the main DLL file.


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



void Geos::Process( __in const char *inputOriginPath, __in const char *inputInteractivePath, __in const char *outputPath, int erosion, int diletation )
{
	cout << erosion << endl;
	HRESULT hr;
	int x = 0;

	x += 2;
	ImageHandler imageHandler( inputOriginPath, outputPath, &hr );

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
			ImageHandler imageHandler( inputInteractivePath, outputPath, &hr );

			if ( SUCCEEDED( hr ) )
			{
				/* Aloc InteractiveImage image */
				Image *pInteractiveImage = new Image(
					imageHandler.InputImageWidth(), 
					imageHandler.InputImageHeight(), 
					imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth(),
					new BYTE[imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth()] 
				);
				hr = imageHandler.Create( pInteractiveImage );
				if ( SUCCEEDED( hr ) )
				{
					ImageSegmentation( const_cast<Image&>( *pInteractiveImage ), erosion, diletation, *pOrigin );
					imageHandler.Save( pOrigin );
				}
				delete pInteractiveImage;
			}
		}
		delete pOrigin;
	}
}


void Geos::GrayScale( __in const Image & rOrigin, __out Image * pGrayImage )
{
	UINT byteDepth = 0;
	if ( rOrigin.size == rOrigin.width * rOrigin.height * 3 )
		byteDepth = 3;
	else if ( rOrigin.size == rOrigin.width * rOrigin.height * 4 )
		byteDepth = 4;
	else if ( rOrigin.size == rOrigin.width * rOrigin.height )
		byteDepth = 1;


	_ASSERT( byteDepth == 1 || byteDepth == 3 || byteDepth == 4 );

	if ( byteDepth == 1 )
	{
		for (int i = 0; i < pGrayImage->size; i++)
		{
			pGrayImage->buffer[i] = rOrigin.buffer[i];
		}
		return;
	}

	UINT j = 0;
	for (int i = 0; i < pGrayImage->size; i++)
	{
		UINT rgbSum = 2989 * rOrigin.buffer[j + 2] + 5870 * rOrigin.buffer[j + 1] + 1140 * rOrigin.buffer[j];
		pGrayImage->buffer[i] = rgbSum / 10000;
		j += byteDepth;
	}
}







void Geos::ImageSegmentation( __in const Image & pInteractiveImage,  __in int erosion, __in int diletation, __inout Image & pOrigin )
{
	double ** ppProbability = new double*[pInteractiveImage.width];
	bool ** result = new bool*[pInteractiveImage.width];
	for (int i = 0; i < pInteractiveImage.width; i++)
	{
		ppProbability[i] = new double[pInteractiveImage.height];
		result[i] = new bool[pInteractiveImage.height];
	}






	//SetPixelProbability( pInteractiveImage, pGrayImage, const_cast<Image&>( pOrigin ), ppProbability );
	//GMM g;
	//g.AutomaticProbability( pOrigin, ppProbability );

	Probability p;
	p.GetProbabitily( SegmentationType::Sharpness, pOrigin, ppProbability );

	MinimizeEnegry( pOrigin, ppProbability, result );



	UINT i = 0, j = 0;
	for (int y = 0; y < pInteractiveImage.height; y++)
	{
		for (int x = 0; x < pInteractiveImage.width; x++)
		{
			if (result[x][y] == false )
			{
				pOrigin.buffer[i] = 0;
				pOrigin.buffer[i + 1] = 0;
				pOrigin.buffer[i + 2] = 0;
			}
			else
			{
				//pOrigin.buffer[i] = 255;
				//pOrigin.buffer[i + 1] = 255;
				//pOrigin.buffer[i + 2] = 255;
			}


			//cout << s.m_ppSignedDistance[x][y] << endl;
				

			//pOrigin.buffer[i] = ppProbability[x][y] * 255.0;
			//pOrigin.buffer[i + 1] = ppProbability[x][y] * 255.0;
			//pOrigin.buffer[i + 2] = ppProbability[x][y] * 255.0;
			i += 3;
			j ++;
		}
	}
}

void Geos::MinimizeEnegry( __in const Image & rOrigin, __in double ** ppProbability, __out bool ** ppLabeling )
{
	Image *pGrayImage = new Image( rOrigin.width, rOrigin.height, rOrigin.width * rOrigin.height, new BYTE[rOrigin.width * rOrigin.height] );
	GrayScale( rOrigin, pGrayImage );
	SymmetricalFilter s( 0.05, pGrayImage, const_cast<const double **>( ppProbability ) );

	int energy = MAXINT32;
	int segmentationIndex;
	for (int i = 0; i < 3; i++)
	{
		s.GetSymmetricalMask( i * 20, ( i + 1 ) * 20, ppLabeling );
		int newEnergy = CountEnegry( const_cast<Image &>( *pGrayImage ), ppProbability, ppLabeling );
		cout << "energi " << newEnergy << endl;
		if ( newEnergy < energy )
		{
			energy = newEnergy; 
			segmentationIndex = i;
		}
	}
	s.GetSymmetricalMask( segmentationIndex * 20, ( segmentationIndex + 1 ) * 20, ppLabeling );
	cout << "teta " << segmentationIndex << endl;
}

double Geos::CountEnegry( __in const Image & rGrayImage, __in double ** ppProbability, __in bool ** ppLabeling )
{
	double energyU = 0.0;
	double energyV = 0.0;
	double teta = 1.0;
	double n = 200.0;

	// Energy of U(z, a)
	for (int y = 0; y < rGrayImage.height; y++)
	{
		for (int x = 0; x < rGrayImage.width; x++)
		{
			if ( ppLabeling[x][y] == true )
				energyU += ppProbability[x][y];
			else 
				energyU += 1.0 - ppProbability[x][y]; 
		}
	}
	energyU *= -1.0;

	// Enegry of V(z, a)
	for (int y = 0; y < rGrayImage.height - 1; y++)
	{
		for (int x = 1; x < rGrayImage.width - 1; x++)
		{
			if ( ppLabeling[x][y] != ppLabeling[x + 1][y] )
				energyV += exp( -1.0 * abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ y * rGrayImage.width + x + 1] ) / n );
			if ( ppLabeling[x][y] != ppLabeling[x - 1][y + 1] )
				energyV += exp( -1.0 * abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x - 1] ) / n ); 
			if ( ppLabeling[x][y] != ppLabeling[x + 0][y + 1] )
				energyV += exp( -1.0 * abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x + 0] ) / n ); 
			if ( ppLabeling[x][y] != ppLabeling[x + 1][y + 1] )
				energyV += exp( -1.0 * abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x + 1] ) / n ); 
		}
	}
	energyV *= -1;

	return energyU + teta * energyV;
}




//----------------------------------------------------------------

void Geos::SetPixelProbability( __in const Image & pInteractiveImage, __in const Image * pGrayImage, __in const Image & pOrigin, __out double ** ppProbability )
{
	bool ** ppLocations = new bool*[pInteractiveImage.width];
	double ** ppBgProbability = new double*[pInteractiveImage.width];
	for (int i = 0; i < pInteractiveImage.width; i++)
	{
		ppLocations[i] = new bool[pInteractiveImage.height];
		ppBgProbability[i] = new double[pInteractiveImage.height];
	}

	/* Foregraung Pixels */
	SetLocations( pInteractiveImage, []( BYTE b, BYTE g, BYTE r ){ return ( b < 80 && g > 180 && r < 80 ); }, ppLocations );
	SetAreaProbability( pGrayImage, pOrigin, ppLocations, ppProbability );
	/* Background Pixels */
	SetLocations( pInteractiveImage, []( BYTE b, BYTE g, BYTE r ){ return ( b < 80 && g < 80 && r > 180 ); }, ppLocations );
	SetAreaProbability( pGrayImage, pOrigin, ppLocations, ppBgProbability );




	for (int y = 0; y < pInteractiveImage.height; y++)
	{
		for (int x = 0; x < pInteractiveImage.width; x++)
		{
			ppProbability[x][y] = ( ppProbability[x][y] - ppBgProbability[x][y] ) + 0.5;
			if (ppProbability[x][y] < 0 ) ppProbability[x][y] = 0;
			if (ppProbability[x][y] > 1 ) ppProbability[x][y] = 1;

			//double log2 = log( 2.0 );
			//double lp = log( ppProbability[x][y] ) / log2 - log( ppBgProbability[x][y] ) / log2;
			//ppProbability[x][y] = 1 / pow( 1 + exp( -lp / 5 ), 3);

		}
	}

	for (int i = 0; i < pInteractiveImage.width; i++)
	{
		delete ppLocations[i];
		delete ppBgProbability[i];
	}
	delete ppLocations;
	delete ppBgProbability;
}

void Geos::SetLocations( __in const Image & pInteractiveImage, __in std::function<bool ( BYTE, BYTE, BYTE )> condition, __out bool ** ppLocations )
{
	UINT i = 0;
	for (int y = 0; y < pInteractiveImage.height; y++)
	{
		for (int x = 0; x < pInteractiveImage.width; x++)
		{
			ppLocations[x][y] = condition( pInteractiveImage.buffer[i], pInteractiveImage.buffer[i + 1], pInteractiveImage.buffer[i + 2] );
			i += 3;
		}
	}
}


void Geos::SetAreaProbability( __in const Image * pGrayImage, __in const Image & pOrigin, __in bool ** ppLocations, __out double ** ppProbability )
{
	double ** ppDistance = new double*[pGrayImage->width];
	for (int i = 0; i < pGrayImage->width; i++)
		ppDistance[i] = new double[pGrayImage->height];

	for (int y = 0; y < pGrayImage->height; y++)
	{
		for (int x = 0; x < pGrayImage->width; x++)
		{
			if (ppLocations[x][y])
				ppDistance[x][y] = 0;
			else
				ppDistance[x][y] = MAXINT32;
		}
	}


	GMM gmm;
	/* Color base probability */
	gmm.InteractiveProbability( pOrigin, ppLocations, ppProbability );

	SymmetricalFilter s(1.8, pGrayImage, const_cast< const double **>( ppDistance ) );
	s.CountUnSignedDistance( pGrayImage, ppDistance );
	s.CountUnSignedDistance( pGrayImage, ppDistance );
	
	double max = 0;
	for (int y = 1; y < pGrayImage->height - 1; y++)
	{ 
		for (int x = 1; x < pGrayImage->width - 1; x++)
		{
			if (ppDistance[x][y] > max)
				max = ppDistance[x][y];
		}
	}

	for (int y = 0; y < pGrayImage->height; y++)
	{
		for (int x = 0; x < pGrayImage->width; x++)
		{
			double distanceBaseProbability = 1.0 - ( ppDistance[x][y] / ( max / 2 ) );
			if (distanceBaseProbability  < 0 ) distanceBaseProbability = 0;
			ppProbability[x][y] =  sqrt( ppProbability[x][y] * distanceBaseProbability );

		}
	}
}


