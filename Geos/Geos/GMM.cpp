#include "stdafx.h"
#include "GMM.h"

#include <iostream>
using namespace std;


GMM::GMM(void)
{
	m_nMaxLoop = 50;

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
	for(int a=0; a<m_nSizeFeature; a++) {
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


void GMM::Train( const Image & rImage, bool ** ppLocation, double ** ppProbability )
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
	INPUTDATA_MULTI_GAUSS **ppData = new INPUTDATA_MULTI_GAUSS*[numberOfRecords];
	UINT i = 0;
	for (int y = 0; y < rImage.height; y++)
	{
		for (int x = 0; x < rImage.width; x++)
		{
			if (ppLocation[x][y]) 
			{
				ppData[i] = new INPUTDATA_MULTI_GAUSS;
				ppData[i]->pNormalProb = new double[2];
				ppData[i]->pData = new double[3];
				UINT base = ( y * rImage.width + x ) * 3;
				ppData[i]->pData[0] = rImage.buffer[base];
				ppData[i]->pData[1] = rImage.buffer[base + 1];
				ppData[i]->pData[2] = rImage.buffer[base + 2];
				ppData[i]->nClass = -1;
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
					dGauss = getgauss2(m_ppMeanFeatClass[b][c], m_ppVarFeatClass[b][c], rImage.buffer[base + c]);
					pProbability[b] *= dGauss;
				}
				dProbSum += pProbability[b] * m_pNumClass[b];
			}

			ppProbability[x][y] = dProbSum / (double)numberOfRecords;
			i += 3;
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

void GMM::init(int nSizeK, int nSizeRecord, int nSizeFeature, INPUTDATA_MULTI_GAUSS ** ppDataList)
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

	for(int z=0; z<m_nMaxLoop; z++) {
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
	for(int a=0; a<m_nSizeRecord; a++) {
		// assign initial cluster(random)
		m_ppDataList[a]->nClass = a % m_nSizeK;
	}

	e_step();
}

// update probability parameters
void GMM::e_step()
{
	// reset statistics
	for(int a=0; a<m_nSizeK; a++) {
		m_pNumClass[a] = 0;
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppSumFeatClass[a][b] = 0; 
			m_ppSumVarClass[a][b] = 0;
		}
	}

	// count record & feature value
	for(int a=0; a<m_nSizeRecord; a++) {
		m_pNumClass[m_ppDataList[a]->nClass]++;
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppSumFeatClass[m_ppDataList[a]->nClass][b] += m_ppDataList[a]->pData[b]; 
		}
	}

	// calc mean
	for(int a=0; a<m_nSizeK; a++) {
		m_pProbClass[a] = (double)((double)m_pNumClass[a] / (double)m_nSizeRecord);
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppMeanFeatClass[a][b] = (double)((double)m_ppSumFeatClass[a][b] / (double)m_pNumClass[a]);
		}
	}

	// calc variance
	for(int a=0; a<m_nSizeRecord; a++) {
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppSumVarClass[m_ppDataList[a]->nClass][b] = m_ppSumVarClass[m_ppDataList[a]->nClass][b] + abs(m_ppDataList[a]->pData[b] - m_ppMeanFeatClass[m_ppDataList[a]->nClass][b]);
		} 
	}

	for(int a=0; a<m_nSizeK; a++) {
		for(int b=0; b<m_nSizeFeature; b++) {
			m_ppVarFeatClass[a][b] = m_ppSumVarClass[a][b] / (double)(m_pNumClass[a]);
		}
	}	
}

// assign clustering
void GMM::m_step()
{
	double * pProbability = new double[m_nSizeK];
	double dGauss = 1, dProbSum = 0;
	
	m_bIsChangeClass = false;

	for(int a=0; a<m_nSizeRecord; a++) {
		dProbSum = 0;

		for(int b=0; b<m_nSizeK; b++) {
			pProbability[b] = 1;
			dGauss = 1;
			
			for(int c=0; c<m_nSizeFeature; c++) {
				dGauss = getgauss2(m_ppMeanFeatClass[b][c], m_ppVarFeatClass[b][c], m_ppDataList[a]->pData[c]);
				pProbability[b] *= dGauss;
			}
			dProbSum += pProbability[b];
		}

		double dTemp = 0;
		int nMaxId = 0;
		for(int b=0; b<m_nSizeK; b++) {
			m_ppDataList[a]->pNormalProb[b] = pProbability[b] / dProbSum;

			int nCurClass = m_ppDataList[a]->nClass;
			if(dTemp < pProbability[b]) {
				nMaxId = b;
				dTemp = pProbability[b];
			}
		}

		if(m_ppDataList[a]->nClass != nMaxId)
		{
			m_bIsChangeClass = true;
		}
		m_ppDataList[a]->nClass = nMaxId;

	}

	delete[] pProbability;
}


inline double GMM::getgauss(double dMean, double dVar, double dValue)
{
	double dGauss = 1;
	const double dPi = 3.14159265358979323846;


	dGauss = (1.0 / sqrt(2.0 * dPi * dVar)) * (exp((-1.0 * (dValue - dMean) * (dValue - dMean)) / (2.0 * dVar)));

	if (_isnan(dGauss)) return 0;

	return dGauss;
}

inline double GMM::getgauss2(double dMean, double dVar, double dValue)
{
	double dGauss = 1;
	const double dPi = 3.14159265358979323846;

	double dif = (dValue - dMean) / 255.0;
	double var = dVar / 255.0;

	dGauss =  (exp(-0.5 * dif * dif / var));

	if (_isnan(dGauss)) return 0;

	return dGauss;
}
