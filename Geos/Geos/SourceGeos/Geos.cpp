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
	for (int i = 0; i < m_width; i++)
	{
	 	delete m_ppResult[i];
	}
	delete m_ppResult;
}



void Geos::Process( 
		__in const char *inputImagePath,
		__in Location * pForeground,
		__in Location * pBackground,
		__in int sharpType,
		__in int timeOptimalization,
		__in int smoothness,
		__in int colorRepresentation
		)
{
	HRESULT hr;
	ImageHandler imageHandler( inputImagePath, "root.jpg", &hr );

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
			SetSegmentationParameters( sharpType, timeOptimalization, smoothness, colorRepresentation );
			ImageSegmentation( const_cast<Image&>( *pOrigin ) );
			//imageHandler.Save( pOrigin );
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







void Geos::ImageSegmentation( __in const Image & rOrigin )
{
	double ** ppProbability = new double*[rOrigin.width];
	bool ** result = new bool*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
	{
		ppProbability[i] = new double[rOrigin.height];
		result[i] = new bool[rOrigin.height];
	}

	//SetPixelProbability( pInteractiveImage, pGrayImage, const_cast<Image&>( pOrigin ), ppProbability );
	//GMM g;
	//g.AutomaticProbability( pOrigin, ppProbability );

	Probability p;
	p.GetProbabitily( m_SegmentationType, rOrigin, ppProbability );

	MinimizeEnegry( rOrigin, ppProbability, result );

	m_width = rOrigin.width;
	m_height = rOrigin.height;
	m_ppResult = new bool*[m_width];
	for (int i = 0; i < m_width; i++)
		m_ppResult[i] = new bool[m_height];

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			m_ppResult[x][y] = result[x][y];
		}
	}



	UINT i = 0, j = 0;
	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			rOrigin.buffer[i] = ppProbability[x][y] * 255.0;
			rOrigin.buffer[i + 1] = ppProbability[x][y] * 255.0;
			rOrigin.buffer[i + 2] = ppProbability[x][y] * 255.0;
			i += 3;
			j ++;
		}
	}
}

void Geos::MinimizeEnegry( __in const Image & rOrigin, __in double ** ppProbability, __out bool ** ppLabeling )
{
	Image *pGrayImage = new Image( rOrigin.width, rOrigin.height, rOrigin.width * rOrigin.height, new BYTE[rOrigin.width * rOrigin.height] );
	GrayScale( rOrigin, pGrayImage );
	SymmetricalFilter s( 0.2, pGrayImage, const_cast<const double **>( ppProbability ) );

	int energy = MAXINT32;
	int segmentationIndex;
	int numberOfEnergies = ( 3 - m_TimeOptimalization ) * 2; // 2, 4, 6
	int smoothness = ( m_Smoothness + 1 ) * 20; // 20, 40, 60

	int tetaDiameter = sqrt( rOrigin.width * rOrigin.height ) / 100 * ( m_Smoothness + 1 );

	for (int i = 1; i <= numberOfEnergies; i++)
	{
		int ans = tetaDiameter + tetaDiameter / i;
		if ( i % 2 == 1 )
			ans = tetaDiameter - tetaDiameter / i;


		s.GetSymmetricalMask( ans, ans + 16, ppLabeling );
		int newEnergy = CountEnegry( const_cast<Image &>( *pGrayImage ), ppProbability, ppLabeling );
		cout << "energi " << newEnergy << endl;
		if ( newEnergy < energy )
		{
			energy = newEnergy; 
			segmentationIndex = i;
		}
	}

	int ans = tetaDiameter + tetaDiameter / segmentationIndex;
	if ( segmentationIndex % 2 == 1 )
		ans = tetaDiameter - tetaDiameter / segmentationIndex;
	s.GetSymmetricalMask( ans, ans + 16, ppLabeling );
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



void Geos::SetSegmentationParameters( __in int sharpType, __in int timeOptimalization, __in int smoothness, __in int colorRepresentation )
{
	_ASSERT( sharpType == 0 || sharpType == 1 );
	_ASSERT( timeOptimalization == 0 || timeOptimalization == 1 || timeOptimalization == 2 );
	_ASSERT( smoothness == 0 || smoothness == 1 || smoothness == 2 );
	_ASSERT( colorRepresentation == 0 || colorRepresentation == 1 );


	if ( sharpType == 0 )
		m_SegmentationType = SegmentationType::Sharpness;
	else
		m_SegmentationType = SegmentationType::Color;

	m_TimeOptimalization = timeOptimalization;
	m_Smoothness = smoothness;

	if ( colorRepresentation == 0 )
		m_RgbType = true;
	else
		m_RgbType = false;
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


