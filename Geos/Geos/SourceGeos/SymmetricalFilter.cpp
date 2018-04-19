#include "stdafx.h"
#include "SymmetricalFilter.h"

#include <iostream>

using namespace std;

const double sqrt2 = 1.41421356237;

/* v from paper eq. 9 */
const double v = 200.0;

SymmetricalFilter::SymmetricalFilter( __in double alfa, __in const Image & rGrayImage ): m_rGrayImage(rGrayImage)
{
	m_NeedToDealoc = false;
	m_Alfa = alfa;
	m_Width = rGrayImage.width;
	m_Height = rGrayImage.height;
}

SymmetricalFilter::SymmetricalFilter( __in double alfa, __in const Image & rGrayImage, __in const double **ppInputProbability ): m_rGrayImage( rGrayImage )
{
	m_NeedToDealoc = true;
	m_Alfa = alfa;
	m_Width = rGrayImage.width;
	m_Height = rGrayImage.height;


	/* Aloc SignedDistance */
	m_ppSignedDistance = new double*[m_Width];
	for (int i = 0; i < m_Width; i++)
	{
		m_ppSignedDistance[i] = new double[m_Height];
	}
	
	CountSignedDistance( ppInputProbability );
}



SymmetricalFilter::~SymmetricalFilter()
{
	if ( m_NeedToDealoc )
	{
		/* Dealoc SignedDistance */
		for (int i = 0; i < m_Width; i++)
		{
			delete[] m_ppSignedDistance[i];
		}
		delete[] m_ppSignedDistance;
	}
}


void SymmetricalFilter::GetSymmetricalMask( __in double erosion, __in double dilation, __out bool **ppSymmetricalMask )
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



void SymmetricalFilter::CountSignedDistance( __in const double **ppInputProbability )
{
	//m_Alfa += 11.0;
	/* Aloc distance to complement */
	double **ppDistanceToObject = new double*[m_Width];
	double **ppDistanceToComplement = new double*[m_Width];
	for (int i = 0; i < m_Width; i++)
	{
		ppDistanceToComplement[i] = new double[m_Height];
		ppDistanceToObject[i] = new double[m_Height];
	}

	InitDistance( false, ppInputProbability, ppDistanceToObject );
	CountUnSignedDistance( ppDistanceToObject, true);
	CountUnSignedDistance( ppDistanceToObject, true);
	CountUnSignedDistance( ppDistanceToObject, true);
	CountUnSignedDistance( ppDistanceToObject, true);

	InitDistance( true, ppInputProbability, ppDistanceToComplement );
	CountUnSignedDistance( ppDistanceToComplement, true);
	CountUnSignedDistance( ppDistanceToComplement, true);
	CountUnSignedDistance( ppDistanceToComplement, true);
	CountUnSignedDistance( ppDistanceToComplement, true);

	/* Count signed distance */
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			m_ppSignedDistance[x][y] = ppDistanceToObject[x][y] - ppDistanceToComplement[x][y];
		}

	/* Dealoc distance to complement */
	for (int i = 0; i < m_Width; i++)
	{
		delete[] ppDistanceToComplement[i];
	}
	delete[] ppDistanceToComplement;
	//m_Alfa -= 11.0;
}



void SymmetricalFilter::CountUnSignedDistance( __inout double **ppDistance, __in bool normalDistance )
{
	int dxa = - m_rGrayImage.width - 1;
	int dxb = - m_rGrayImage.width; 
	int dxc = - m_rGrayImage.width + 1;
	int dxd = - 1; 
	int index = m_rGrayImage.width;
	double da, db, dc, dd;
	double d1, d2, d3, d4;

	for (int y = 1; y < m_rGrayImage.height; y++)
	{
		index++;
		for (int x = 1; x < m_rGrayImage.width; x++)
		{
			if ( normalDistance )
				GrayBaseNeighborDistances( m_rGrayImage.buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );
			else
				GrayBaseNeighborDistancesSharpArea( m_rGrayImage.buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );


			d1 = sqrt2 + m_Alfa * da + ppDistance[x - 1][y - 1];
			d2 = 1.0 + m_Alfa * db + ppDistance[x][y - 1];
			if ( x != m_rGrayImage.width - 1 )
				d3 = sqrt2 + m_Alfa * dc + ppDistance[x + 1][y - 1];
			else
				d3 = MAXINT32;
			d4 = 1.0 + m_Alfa * dd + ppDistance[x - 1][y];

			SetMinimum( d1, d2, d3, d4, &ppDistance[x][y] );
			index++;
		}
	}
	index -= m_rGrayImage.width;
	dxa = 1;
	dxb = m_rGrayImage.width - 1; 
	dxc = m_rGrayImage.width;
	dxd = m_rGrayImage.width + 1; 

	for (int y = m_rGrayImage.height - 2; y >= 0; y--)
	{
		index--;
		for (int x = m_rGrayImage.width - 2; x >= 0; x--)
		{
			index--;
			if ( normalDistance )
				GrayBaseNeighborDistances( m_rGrayImage.buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );
			else
				GrayBaseNeighborDistancesSharpArea( m_rGrayImage.buffer, index, dxa, dxb, dxc, dxd, &da, &db, &dc, &dd );


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



void SymmetricalFilter::InitDistance( __in bool toComplement, __in const double **ppInputProbability, __out double **ppDistance )
{
	if ( toComplement )
	{
		for (int y = 0; y < m_Height; y++)
			for (int x = 0; x < m_Width; x++)
			{
				ppDistance[x][y] = v * ppInputProbability[x][y] ;
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



void SymmetricalFilter::CountErosion( __in double erosion, __out double **ppErosionDistance )
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			if ( m_ppSignedDistance[x][y] > - erosion )
				ppErosionDistance[x][y] = MAXINT32;
			else 
				ppErosionDistance[x][y] = 0;
		}

	for (int i = 0; i < 8; i++)
		CountUnSignedDistance( ppErosionDistance, true);
}

void SymmetricalFilter::CountDilation( __in double dilation, __out double **ppDilationDistance )
{
	for (int y = 0; y < m_Height; y++)
		for (int x = 0; x < m_Width; x++)
		{
			if ( m_ppSignedDistance[x][y] > dilation )
				ppDilationDistance[x][y] = 0;
			else 
				ppDilationDistance[x][y] = MAXINT32;
		}

	for (int i = 0; i < 8; i++)
		CountUnSignedDistance( ppDilationDistance, true);

}