#include "stdafx.h"
#include "SymmetricalFilter.h"

#include <iostream>

using namespace std;

const double sqrt2 = 1.41421356237;

/* v from paper eq. 9 */
const double v = 100.0;

SymmetricalFilter::SymmetricalFilter( __in double alfa, __in const Image *pGrayImage, __in double **ppInputProbability )
{
	m_Alfa = alfa;
	m_pGrayImage = pGrayImage;
	m_Width = pGrayImage->width;
	m_Height = pGrayImage->height;


	/* Aloc SignedDistance */
	m_ppSignedDistance = new double*[m_Width];
	for (int i = 0; i < m_Width; i++)
	{
		m_ppSignedDistance[i] = new double[m_Height];
	}
	
	CountSignedDistance( pGrayImage, ppInputProbability );
}



SymmetricalFilter::~SymmetricalFilter()
{
	/* Dealoc SignedDistance */
	for (int i = 0; i < m_Width; i++)
	{
		delete[] m_ppSignedDistance[i];
	}
	delete[] m_ppSignedDistance;
}


void SymmetricalFilter::GetSymmetricalMask( __in int erosion, __in int dilation, __out bool **ppSymmetricalMask )
{
	/* Aloc dilation and erosion distance. All neede memory */
	double **ppErosionDistance = new double*[m_Width];
	double **ppDilationDistance = new double*[m_Width];
	for (int i = 0; i < m_Width; i++)
	{
		ppErosionDistance[i] = new double[m_Height];
		ppDilationDistance[i] = new double[m_Height];
	}

	/* Distance from inputMask erosion */
	CountErosion( erosion, ppErosionDistance );



	/* Distance from inputMask dilation */
	CountDilation( dilation, ppDilationDistance );


	/* Set result mask */
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			if ( ppErosionDistance[x][y] - ppDilationDistance[x][y] + dilation - erosion > 0 )
				ppSymmetricalMask[x][y] = false;
			else 
				ppSymmetricalMask[x][y] = true;
		}

	/* Dealoc dilation and erosion distance */
	for (int i = 0; i < m_Width; i++)
	{
		delete[] ppDilationDistance[i];
		delete[] ppErosionDistance[i];
	}
	delete[] ppDilationDistance;
	delete[] ppErosionDistance;
}



void SymmetricalFilter::CountSignedDistance( __in const Image *pGrayImage, __in double **ppInputProbability )
{
	/* Aloc distance to complement */
	double **ppDistanceToComplement = new double*[m_Width];
	for (int i = 0; i < m_Width; i++)
	{
		ppDistanceToComplement[i] = new double[m_Height];
	}

	InitDistance( false, ppInputProbability, m_ppSignedDistance );
	CountUnSignedDistance( pGrayImage, m_ppSignedDistance );

	InitDistance( true, ppInputProbability, ppDistanceToComplement );
	CountUnSignedDistance( pGrayImage, ppDistanceToComplement );

	/* Count signed distance */
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			m_ppSignedDistance[x][y] -= ppDistanceToComplement[x][y];
		}

	/* Dealoc distance to complement */
	for (int i = 0; i < m_Width; i++)
	{
		delete[] ppDistanceToComplement[i];
	}
	delete[] ppDistanceToComplement;
}



void SymmetricalFilter::CountUnSignedDistance( __in const Image *pGrayImage, __inout double **ppDistance )
{
	int dxa = - pGrayImage->width - 1;
	int dxb = - pGrayImage->width; 
	int dxc = - pGrayImage->width + 1;
	int dxd = - 1; 
	int index = pGrayImage->width;
	double da, db, dc, dd;
	double d1, d2, d3, d4;

	for (int y = 1; y < pGrayImage->height; y++)
	{
		index++;
		for (int x = 1; x < pGrayImage->width; x++)
		{
			GrayBaseNeighborDistances( pGrayImage->buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );

			d1 = sqrt2 + m_Alfa * da + ppDistance[x - 1][y - 1];
			d2 = 1.0 + m_Alfa * db + ppDistance[x][y - 1];
			if ( x != pGrayImage->width - 1 )
				d3 = sqrt2 + m_Alfa * dc + ppDistance[x + 1][y - 1];
			else
				d3 = MAXINT32;
			d4 = 1.0 + m_Alfa * dd + ppDistance[x - 1][y];

			SetMinimum( d1, d2, d3, d4, &ppDistance[x][y] );
			index++;
		}
	}
	index -= pGrayImage->width;
	dxa = 1;
	dxb = pGrayImage->width - 1; 
	dxc = pGrayImage->width;
	dxd = pGrayImage->width + 1; 

	for (int y = pGrayImage->height - 2; y >= 0; y--)
	{
		index--;
		for (int x = pGrayImage->width - 2; x >= 0; x--)
		{
			index--;
			GrayBaseNeighborDistances( pGrayImage->buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );

			d1 = 1.0 + m_Alfa * da + ppDistance[x + 1][y];
			if ( x != 0 )
				d2 = sqrt2 + m_Alfa * db + ppDistance[x - 1][y + 1];
			else
				d2 = MAXINT32;
			d3 = 1.0 + m_Alfa * dc + ppDistance[x][y + 1];
			d4 = sqrt2 + m_Alfa * dd + ppDistance[x + 1][y + 1];

			SetMinimum( d1, d2, d3, d4, &ppDistance[x][y] );
		}
	}
}



void SymmetricalFilter::InitDistance( __in bool toComplement, __in double **ppInputProbability, __out double **ppDistance )
{
	if ( toComplement )
	{
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				ppDistance[x][y] = v * ppInputProbability[x][y];
			}
	}
	else
	{
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				ppDistance[x][y] = v * ( 1.0 - ppInputProbability[x][y] );
			}
	}
}



void SymmetricalFilter::CountErosion( __in int erosion, __out double **ppErosionDistance )
{
	int enou = 0, eyes = 0;
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
			if ( m_ppSignedDistance[x][y] > - erosion )
				ppErosionDistance[x][y] = MAXINT32;
			else 
				ppErosionDistance[x][y] = 0;

	CountUnSignedDistance( m_pGrayImage, ppErosionDistance );
}

void SymmetricalFilter::CountDilation( __in int dilation, __out double **ppDilationDistance )
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
			if ( m_ppSignedDistance[x][y] > dilation )
				ppDilationDistance[x][y] = 0;
			else 
				ppDilationDistance[x][y] = MAXINT32;

	CountUnSignedDistance( m_pGrayImage, ppDilationDistance );
}