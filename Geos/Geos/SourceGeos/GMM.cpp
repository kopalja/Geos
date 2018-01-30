#pragma once
#include "GMM.h"

#include <iostream>
using namespace std;

const double M_PI = 3.14159265358979323846264338327950288;


GMM::GMM(void)
{
	m_nMaxLoop = 20;

	m_pNumClass = 0;
	m_pProbClass = 0;

	m_ppSumFeatClass = 0;
	m_ppSumVarClass = 0;
	m_ppMeanFeatClass = 0;
	m_ppVarFeatClass = 0;
}

GMM::~GMM(void)
{
	delete[] m_pNumClass;
	delete[] m_pProbClass;
	return;
	for(int a=0; a<m_nSizeFeature; a++) 
	{
		delete[] m_ppSumFeatClass[a];
		delete[] m_ppSumVarClass[a];
		delete[] m_ppMeanFeatClass[a];
		delete[] m_ppVarFeatClass[a];
	}
	delete[] m_ppSumFeatClass;
	delete[] m_ppSumVarClass;
	delete[] m_ppMeanFeatClass;
	delete[] m_ppVarFeatClass;
}			

void GMM::AutomaticProbability( __in const Image & rImage, __out double ** ppProbability )
{

	double * pLab = new double[3];
	INPUTDATA_MULTI_GAUSS * ppData = new INPUTDATA_MULTI_GAUSS[rImage.width * rImage.height];
	UINT i = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			RgbToLab( rImage.buffer[( i * 3 ) + 2], rImage.buffer[( i * 3 ) + 1], rImage.buffer[( i * 3 ) + 0], pLab );

			ppData[i].L = rImage.buffer[( i * 3 ) + 2];
			ppData[i].A = rImage.buffer[( i * 3 ) + 1];
			ppData[i].B = rImage.buffer[( i * 3 ) + 0];

			ppData[i].L = pLab[0];
			ppData[i].A = pLab[1];
			ppData[i].B = pLab[2];

			ppData[i].nClass = -1;
			i++;
		}
	}
	init( 2, rImage.width * rImage.height, 3, ppData );
	train();

	i = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			double *pProbability = new double[m_nSizeK];
			for( int b = 0; b < m_nSizeK; b++ ) 
			{
				pProbability[b] = 1;
				pProbability[b] *= getgauss( m_ppMeanFeatClass[b][0], m_ppVarFeatClass[b][0], ppData[i].L );
				pProbability[b] *= getgauss( m_ppMeanFeatClass[b][1], m_ppVarFeatClass[b][1], ppData[i].A );
				pProbability[b] *= getgauss( m_ppMeanFeatClass[b][2], m_ppVarFeatClass[b][2], ppData[i].B );
			}

			ppProbability[x][y] = log( pProbability[0] ) - log( pProbability[1] );
			ppProbability[x][y] = 1.0 / ( 1.0 + exp( - ppProbability[x][y] / 10.0 ) );

			i++;
		}
	}

	cout << "Mean" << endl;
	cout << m_ppMeanFeatClass[0][0]<< endl;
	cout << m_ppMeanFeatClass[0][1] << endl;
	cout << m_ppMeanFeatClass[0][2] << endl;
	cout << "--------" << endl;

	cout << m_ppMeanFeatClass[1][0]<< endl;
	cout << m_ppMeanFeatClass[1][1] << endl;
	cout << m_ppMeanFeatClass[1][2] << endl;
	cout << "--------" << endl;


	cout << "Variance" << endl;
	cout << m_ppVarFeatClass[0][0]<< endl;
	cout << m_ppVarFeatClass[0][1] << endl;
	cout << m_ppVarFeatClass[0][2] << endl;

	cout << "--------" << endl;
	cout << m_ppVarFeatClass[1][0]<< endl;
	cout << m_ppVarFeatClass[1][1] << endl;
	cout << m_ppVarFeatClass[1][2] << endl;
	cout << "--------" << endl;
	cout << "NUmber in gaus" << endl;
	cout << m_pNumClass[0] << endl;
	cout << m_pNumClass[1] << endl;
}

void GMM::InteractiveProbability( const Image & rImage, bool ** ppLocation, double ** ppProbability )
{
	UINT numberOfRecords = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			ppProbability[x][y] = 1;
			if (ppLocation[x][y]) 
				numberOfRecords++;
		}
	}
	INPUTDATA_MULTI_GAUSS * ppData = new INPUTDATA_MULTI_GAUSS[numberOfRecords];
	UINT i = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			if (ppLocation[x][y]) 
			{
				UINT base = ( y * rImage.width + x ) * 3;
				ppData[i].B = rImage.buffer[base];
				ppData[i].A = rImage.buffer[base + 1];
				ppData[i].L = rImage.buffer[base + 2];
				ppData[i].nClass = -1;
				i++;
			}
		}
	}

	init( 2, numberOfRecords, 3, ppData );
	train();

	i = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			int base =  ( y * rImage.width + x ) * 3;
			double *pProbability = new double[m_nSizeK];
			double dProbSum = 0;
			for(int b=0; b<m_nSizeK; b++) 
			{
				pProbability[b] = 1;
				double dGauss = 1;
			
				for(int c=0; c<m_nSizeFeature; c++) 
				{
					dGauss = getgauss(m_ppMeanFeatClass[b][c] / 255.0, m_ppVarFeatClass[b][c] / 255.0, rImage.buffer[base + c] / 255.0 );
					pProbability[b] *= dGauss;
				}
				dProbSum += pProbability[b];
			}
			ppProbability[x][y] = dProbSum;
			i += 3;
		}

	}
}



void GMM::init(int nSizeK, int nSizeRecord, int nSizeFeature, INPUTDATA_MULTI_GAUSS * ppDataList)
{
	m_nSizeK = nSizeK;
	m_nSizeRecord = nSizeRecord;
	m_nSizeFeature = nSizeFeature;
	m_ppDataList = ppDataList;

	m_pNumClass = new int[m_nSizeK];
	m_pProbClass = new double[m_nSizeK];
	m_ppSumFeatClass = new double*[m_nSizeK];
	m_ppSumVarClass = new double*[m_nSizeK];
	m_ppMeanFeatClass = new double*[m_nSizeK];
	m_ppVarFeatClass = new double*[m_nSizeK];
	for(int a=0; a<m_nSizeK; a++) {
		m_pNumClass[a] = 0;
		m_pProbClass[a] = 0;

		m_ppSumFeatClass[a] = new double[m_nSizeFeature];
		m_ppSumVarClass[a] = new double[m_nSizeFeature];
		m_ppMeanFeatClass[a] = new double[m_nSizeFeature];
		m_ppVarFeatClass[a] = new double[m_nSizeFeature];
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppSumFeatClass[a][b] = 0;
			m_ppSumVarClass[a][b] = 0;
			m_ppMeanFeatClass[a][b] = 0;
			m_ppVarFeatClass[a][b] = 0;
		}
	}
}

void GMM::train()
{
	i_step();
	for(int z=0; z<m_nMaxLoop; z++) 
	{
		m_step();
		if(!m_bIsChangeClass)
		{
			cout << "aaaaaa" << endl;
			break;
		}
		e_step();
	}
}

// initalize centroid
void GMM::i_step()
{
	for(int a=0; a< m_nSizeRecord / 2; a++) 
	{
		// assign initial cluster(random)
		m_ppDataList[a].nClass = 0;
	}
	for(int a=m_nSizeRecord / 2; a< m_nSizeRecord; a++) 
	{
		// assign initial cluster(random)
		m_ppDataList[a].nClass = 1;
	}

	e_step();
}

// update probability parameters
void GMM::e_step()
{
	// reset statistics
	for(int a=0; a<m_nSizeK; a++) 
	{
		m_pNumClass[a] = 0;
		for(int b=0; b<m_nSizeFeature; b++) 
		{
			m_ppSumFeatClass[a][b] = 0; 
			m_ppSumVarClass[a][b] = 0;
		}
	}

	// count record & feature value
	for(int a=0; a<m_nSizeRecord; a++) 
	{
		m_pNumClass[m_ppDataList[a].nClass]++;


		m_ppSumFeatClass[m_ppDataList[a].nClass][0] += m_ppDataList[a].L; 
		m_ppSumFeatClass[m_ppDataList[a].nClass][1] += m_ppDataList[a].A;
		m_ppSumFeatClass[m_ppDataList[a].nClass][2] += m_ppDataList[a].B;
	}

	// calc mean
	for(int a=0; a<m_nSizeK; a++) 
	{
		m_pProbClass[a] = (double)((double)m_pNumClass[a] / (double)m_nSizeRecord);
		for(int b=0; b<m_nSizeFeature; b++) 
		{
			if ( m_pNumClass[a] == 0 )
				m_ppMeanFeatClass[a][b] = (double)m_ppSumFeatClass[a][b];
			else
				m_ppMeanFeatClass[a][b] = (double)((double)m_ppSumFeatClass[a][b] / (double)m_pNumClass[a]);
		}
	}

	// calc variance
	for(int a=0; a<m_nSizeRecord; a++) 
	{
		m_ppSumVarClass[m_ppDataList[a].nClass][0] +=  abs( m_ppDataList[a].L - m_ppMeanFeatClass[m_ppDataList[a].nClass][0] );
		m_ppSumVarClass[m_ppDataList[a].nClass][1] += abs( m_ppDataList[a].A - m_ppMeanFeatClass[m_ppDataList[a].nClass][1] );
		m_ppSumVarClass[m_ppDataList[a].nClass][2] += abs( m_ppDataList[a].B - m_ppMeanFeatClass[m_ppDataList[a].nClass][2] );
	}

	for(int a=0; a<m_nSizeK; a++) 
	{
		for(int b=0; b<m_nSizeFeature; b++) 
		{
			if ( m_pNumClass[a] == 0 )
				m_ppVarFeatClass[a][b] = m_ppSumVarClass[a][b];
			else
				m_ppVarFeatClass[a][b] = m_ppSumVarClass[a][b] / (double)(m_pNumClass[a]);
		}
	}	
}

// assign clustering
void GMM::m_step()
{
	double * pProbability = new double[m_nSizeK];
	
	m_bIsChangeClass = false;

	for(int a=0; a<m_nSizeRecord; a++) 
	{
		for(int b=0; b<m_nSizeK; b++) 
		{
			pProbability[b] = 1;
			pProbability[b] *= getgauss( m_ppMeanFeatClass[b][0], m_ppVarFeatClass[b][0], m_ppDataList[a].L );
			pProbability[b] *= getgauss( m_ppMeanFeatClass[b][1], m_ppVarFeatClass[b][1], m_ppDataList[a].A );
			pProbability[b] *= getgauss( m_ppMeanFeatClass[b][2], m_ppVarFeatClass[b][2], m_ppDataList[a].B );
		}

		double dTemp = 0;
		int nMaxId = 0;
		for(int b=0; b<m_nSizeK; b++) 
		{
			int nCurClass = m_ppDataList[a].nClass;
			if(dTemp < pProbability[b]) 
			{
				nMaxId = b;
				dTemp = pProbability[b];
			}
		}

		if(m_ppDataList[a].nClass != nMaxId)
		{
			m_bIsChangeClass = true;
		}
		m_ppDataList[a].nClass = nMaxId;

	}
	delete[] pProbability;
}


inline double GMM::getgauss(double dMean, double dVar, double dValue)
{
	double dGauss = 1;
	const double dPi = 3.14159265358979323846;

	double dif = dValue - dMean;
	double var = dVar;

	//dGauss =   (exp(-0.5 * dif * dif / var));
	dGauss =   (1.0 / sqrt(2.0 * dPi * var)) * (exp(-0.5 * dif * dif / var));
	//dGauss = (1.0 / sqrt(2.0 * dPi * dVar)) * (exp((-1.0 * (dValue - dMean) * (dValue - dMean)) / (2.0 * dVar)));



	if (_isnan(dGauss)) return 0;

	return dGauss;
}




inline void GMM::RgbToLab( __in BYTE red, __in BYTE green, __in BYTE blue, __out_bcount(3) double * pLab )
{
	 double r = red / 255.0;
     double g = green / 255.0;
     double b = blue / 255.0;
	 double x,y,z;

	  r = (r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92;
	  g = (g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92;
	  b = (b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92;

	  x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
	  y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
	  z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;

	  x = (x > 0.008856) ? pow(x, 1.0 / 3.0) : (7.787 * x) + 16.0 / 116.0;
	  y = (y > 0.008856) ? pow(y, 1.0 / 3.0) : (7.787 * y) + 16.0 / 116.0;
	  z = (z > 0.008856) ? pow(z, 1.0 / 3.0) : (7.787 * z) + 16.0 / 116.0;

	  pLab[0] = (116.0 * y) - 16.0;
	  pLab[1] = 500.0 * (x - y);
	  pLab[2] = 200.0 * (y - z);
}

