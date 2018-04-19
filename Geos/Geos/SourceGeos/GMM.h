#pragma once
#include <math.h>
#include "ImageHandler.h"
#include "ExtendedFunctions.h"
#include <vector>

#include <iostream>
using namespace std;

typedef struct inputdata_multi_gauss {
	double L;
	double A;
	double B;
	int nClass;
} INPUTDATA_MULTI_GAUSS;

class Location
{
public:
	inline Location(){};

	inline Location( int x, int y )
	{
		this->x = x;
		this->y = y;
	}
	int x;
	int y;
};


ref class GMM
{
public:


	GMM( 
		__in bool rgbType
	);
	~GMM(void);

	void AutomaticProbability(
		__in const Image & rImage,
		__out double ** ppProbability
		);


	void InteractiveProbability(
		__in const Image & rImage,
		__in const vector<Location> & rForeGround,
		__in const vector<Location> & rBackGround,
		__out double ** ppProbability
		);

	void init(
		__in int nSizeK, 
		__in int nSizeFeature
		);

	void train(
		__in INPUTDATA_MULTI_GAUSS * ppData,
		__in int dataSize
		);

private:

	void CreateModel(
		__in const Image & rImage,
		__in int numberOfGaussian,
		__in const vector<Location> & rTrainData,
		__out double ** ppProbabilityInModel
		);

	void i_step(
		);

	void e_step(
		);

	void m_step(
		);


	inline double getgauss(
		__in double dMean, 
		__in double dVar,
		__in double dValue
		);

	inline double getgaussFast(
		__in double dMean, 
		__in double dVar,
		__in double var,
		__in double dValue
		);

	inline void RgbToLab( 
		__in BYTE r, 
		__in BYTE g,
		__in BYTE b,
		__out_bcount(3) double * pLab 
		);

	int m_nMaxLoop;

	// datas for training
	int m_nSizeK;							// number of output pattern
	int m_nSizeFeature;						// number of feature(column)
	int m_nSizeRecord;						// number of records
	INPUTDATA_MULTI_GAUSS * m_ppDataList;	// input datas of records

	bool m_bIsChangeClass;

	int * m_pNumClass;						// number of records in each class
	double ** m_ppSumFeatClass;				// sum value of feature value in each class
	double ** m_ppSumVarClass;				// sum value of feature variation square in each class
	double ** m_ppMeanFeatClass;			// mean value of feature in each class
	double ** m_ppVarFeatClass;				// covariation value of feature in each class

	double * m_pProbClass;					// 𝑷(𝒄)

	//double getGaussVar[2][3];
	double ** getGaussVar;
	bool m_RgbType;

	
};

