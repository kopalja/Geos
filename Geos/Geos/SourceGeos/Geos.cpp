// This is the main DLL file.


#include "Geos.h"
#include "Energy.h"
#include <iostream>
#include <ctime>

#include <thread>
using namespace std;


const double sqrt2 = 1.41421356237;




Geos::Geos()
{

}

Geos::~Geos()
{
	for (int i = 0; i < m_width; i++)
	{
	 	delete[] m_ppResult[i];
	}
	delete[] m_ppResult;
}




void Geos::Process(
	__in const char *inputImagePath,
	__in const vector<Location> & rForeGround,
	__in const vector<Location> & rBackGround,
	__in int sharpType,
	__in int timeOptimalization,
	__in int smoothness,
	__in int colorRepresentation
)
{
	HRESULT hr;
	ImageHandler imageHandler(inputImagePath, "temp.jpg", &hr);

	if (SUCCEEDED(hr))
	{
		/* Aloc origin image */
		Image *pOrigin = new Image(
			imageHandler.InputImageWidth(),
			imageHandler.InputImageHeight(),
			imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth(),
			new BYTE[imageHandler.InputImageWidth() * imageHandler.InputImageHeight() * imageHandler.InputImageByteDepth()]
		);
		hr = imageHandler.Create(pOrigin);
		if (SUCCEEDED(hr))
		{
			SetSegmentationParameters(sharpType, timeOptimalization, smoothness, colorRepresentation);
			ImageSegmentation(const_cast<Image&>(*pOrigin), rForeGround, rBackGround);
			imageHandler.Save(pOrigin);
		}
		delete pOrigin;
	}
}




void Geos::ImageSegmentation( __in const Image & rOrigin, __in const vector<Location> & rForeGround, __in const vector<Location> & rBackGround )
{
	m_width = rOrigin.width;
	m_height = rOrigin.height;
	m_ppResult = new bool*[m_width];
	for (int i = 0; i < m_width; i++)
		m_ppResult[i] = new bool[m_height];

	clock_t start = clock();
	m_pGrayImage = new Image( rOrigin.width, rOrigin.height, rOrigin.width * rOrigin.height, new BYTE[rOrigin.width * rOrigin.height] );
	GrayScale( rOrigin, m_pGrayImage );

	double ** ppProbability = new double*[rOrigin.width];
	for (int i = 0; i < rOrigin.width; i++)
		ppProbability[i] = new double[rOrigin.height];

	Probability p;
	p.GetProbabitily( m_SegmentationType, m_RgbType, rOrigin, const_cast<Image &>( *m_pGrayImage ), rForeGround, rBackGround, ppProbability );
	cout << "probabilities " << clock() - start << endl;

	MinimizeEnegry( rOrigin, ppProbability );

	cout << "minimalization " << clock() - start << endl;




	UINT i = 0, j = 0;
	for (int y = 0; y < rOrigin.height; y++)
	{
		for (int x = 0; x < rOrigin.width; x++)
		{
			if (m_ppResult[x][y])
			{
				rOrigin.buffer[i] = 255.0;
				rOrigin.buffer[i + 1] = 255.0;
				rOrigin.buffer[i + 2] = 255.0;
			}
			else
			{
				rOrigin.buffer[i] = 0.0;
				rOrigin.buffer[i + 1] = 0.0;
				rOrigin.buffer[i + 2] = 0.0;
			}

			rOrigin.buffer[i] = 255.0 * ppProbability[x][y];
			rOrigin.buffer[i + 1] = 255.0 * ppProbability[x][y];
			rOrigin.buffer[i + 2] = 255.0 * ppProbability[x][y];
			i += 3;
			j ++;
		}
	}
}

void Geos::MinimizeEnegry( __in const Image & rOrigin, __in double ** ppProbability )
{
	SymmetricalFilter * symmetricalFilter = new SymmetricalFilter(0.4, const_cast<Image &>(*m_pGrayImage), const_cast<const double **>(ppProbability));
	int numberOfEnergies = ( 3 - m_TimeOptimalization ) * 2; // 2, 4, 6
	int tetaDiameter = sqrt( rOrigin.width * rOrigin.height ) / 100 * ( ( m_Smoothness * 5 ) + 1 );

	

	vector<Energy*> energy = vector<Energy*>(numberOfEnergies);
	vector<thread> threads = vector<thread>(numberOfEnergies);

	/* Create threads */
	for (size_t i = 0; i < numberOfEnergies; i++)
	{
		energy[i] = new Energy(i * 5, symmetricalFilter, ppProbability, const_cast<Image &>(*m_pGrayImage));
		threads[i] = thread(&Energy::EntryPoint, energy[i]);
	}

	/* Wait for all threads */
	for (size_t i = 0; i < numberOfEnergies; i++)
	{
		threads[i].join();
	}

	/* Set labeling with min energy */
	int minEnergy = MAXINT32;
	for (size_t i = 0; i < numberOfEnergies; i++)
	{
		//cout << "enery " << (*energy)[i]->value << endl;
		if (energy[i]->value < minEnergy)
			minEnergy = energy[i]->value;
	}
	for (size_t i = 0; i < numberOfEnergies; i++)
	{
		if (energy[i]->value == minEnergy)
			CopyLabeling(rOrigin.width, rOrigin.height, energy[i]->ppLabeling);
		delete energy[i];
	}

	delete m_pGrayImage;
}

double Geos::CountEnegry( __in const Image & rGrayImage, __in double ** ppProbability, __in bool ** ppLabeling )
{
	double energyU = 0.0;
	double energyV = 0.0;
	double teta = 1.0;
	double n = 4.0;

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
	double temp;
	// Enegry of V(z, a)
	for (int y = 0; y < rGrayImage.height - 1; y++)
	{
		for (int x = 1; x < rGrayImage.width - 1; x++)
		{
			temp = -1.0 * ( abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ y * rGrayImage.width + x + 1] ) >> 2 );
			if ( ppLabeling[x][y] != ppLabeling[x + 1][y] )
				energyV += expApproximation( -1 * ( abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ y * rGrayImage.width + x + 1] ) >> 2 ) );
			if ( ppLabeling[x][y] != ppLabeling[x - 1][y + 1] )
				energyV += expApproximation( -1 * ( abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x - 1] ) >> 2 ) ); 
			if ( ppLabeling[x][y] != ppLabeling[x + 0][y + 1] )
				energyV += expApproximation( -1 * ( abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x + 0] ) >> 2 ) ); 
			if ( ppLabeling[x][y] != ppLabeling[x + 1][y + 1] )
				energyV += expApproximation( -1 * ( abs( rGrayImage.buffer[ y * rGrayImage.width + x] - rGrayImage.buffer[ (y + 1) * rGrayImage.width + x + 1] ) >> 2 ) ); 
		}
	}
	energyV *= -1;


	return energyU + teta * energyV;
}



void Geos::SetSegmentationParameters( __in int sharpType, __in int timeOptimalization, __in int smoothness, __in int colorRepresentation )
{
	_ASSERT( sharpType == 0 || sharpType == 1 || sharpType == 2 );
	_ASSERT( timeOptimalization == 0 || timeOptimalization == 1 || timeOptimalization == 2 );
	_ASSERT( smoothness == 0 || smoothness == 1 || smoothness == 2 );
	_ASSERT( colorRepresentation == 0 || colorRepresentation == 1 );


	if ( sharpType == 0 )
		m_SegmentationType = SegmentationType::Sharpness;
	else if ( sharpType == 1 )
		m_SegmentationType = SegmentationType::Color;
	else if ( sharpType == 2 )
		m_SegmentationType = SegmentationType::SharpAndColor;

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

			double log2 = log( 2.0 );
			double lp = log( ppProbability[x][y] ) / log2 - log( ppBgProbability[x][y] ) / log2;
			ppProbability[x][y] = 1 / pow( 1 + exp( -lp / 5 ), 3);

		}
	}

	for (int i = 0; i < pInteractiveImage.width; i++)
	{
		delete[] ppLocations[i];
		delete[] ppBgProbability[i];
	}
	delete[] ppLocations;
	delete[] ppBgProbability;
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


	GMM gmm(true);
	/* Color base probability */
	//gmm.InteractiveProbability( pOrigin, ppLocations, ppProbability );

	SymmetricalFilter s(1.8, const_cast<Image &>( *pGrayImage ), const_cast< const double **>( ppDistance ) );
	//s.CountUnSignedDistance( pGrayImage, ppDistance );
	//s.CountUnSignedDistance( pGrayImage, ppDistance );
	
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


