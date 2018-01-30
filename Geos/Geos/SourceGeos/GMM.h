#pragma once
#include <math.h>
#include "ImageHandler.h"

#include <iostream>
using namespace std;

typedef struct inputdata_multi_gauss {
	double L;
	double A;
	double B;
	int nClass;
} INPUTDATA_MULTI_GAUSS;

class GMM
{
public:
	GMM(void);
	~GMM(void);

	void AutomaticProbability(
		__in const Image & rImage,
		__out double ** ppProbability
		);


	void InteractiveProbability(
		__in const Image & rImage,
		__in bool ** ppLocation,
		__out double ** ppProbability
		);

	void init(
		__in int nSizeK, 
		__in int nSizeRecord, 
		__in int nSizeFeature,
		__in_bcount( numberOfPixels ) INPUTDATA_MULTI_GAUSS * ppDataList
		);

	void train(
		);

private:

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
};

