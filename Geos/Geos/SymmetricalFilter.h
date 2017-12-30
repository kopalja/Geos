
#include <Windows.h>
#include "ImageHandler.h"




class SymmetricalFilter
{
public:

	SymmetricalFilter(
		__in double alfa,
		__in const Image *pGrayImage, 
		__in double **ppInputProbability
		);

	SymmetricalFilter( double alfa ){ m_Alfa = alfa; }

	~SymmetricalFilter(
		);

	void GetSymmetricalMask(
		__in int erosion, 
		__in int dilation, 
		__out bool **ppSymmetricalMask 
		);

	void CountUnSignedDistance(
		__in const Image *pGrayImage, 
		__inout double **ppDistance
		);

	double **m_ppSignedDistance;

private:
	double m_Alfa;
	UINT m_Width, m_Height;
	const Image *m_pGrayImage;


	void CountSignedDistance(
		__in const Image *pGrayImage, 
		__in double **ppInputProbability
		);


	void InitDistance(
		__in bool toComplement,
		__in double **ppInputProbability,
		__out double **ppDistance
		);

	void CountErosion(
		__in int erosion,
		__out double **ppErosionDistance
		);

	void CountDilation(
		__in int dilation,
		__out double **ppDilationDistance
		);


	//-----------Inline--methods---------------------------------

	inline void GrayBaseNeighborDistances(
		__in const BYTE *pBuffer,
		__in UINT index,
		__in int dxa,
		__in int dxb,
		__in int dxc,
		__in int dxd,
		__out double *da,
		__out double *db,
		__out double *dc,
		__out double *dd
		)
	{
		int i, mask;
		i = pBuffer[index] - pBuffer[index + dxa];
		mask = i >> shift;
		*da = (i + mask) ^ mask;

		i = pBuffer[index] - pBuffer[index + dxb];
		mask = i >> shift;
		*db = (i + mask) ^ mask;

		i = pBuffer[index] - pBuffer[index + dxc];
		mask = i >> shift;
		*dc = (i + mask) ^ mask;

		i = pBuffer[index] - pBuffer[index + dxd];
		mask = i >> shift;
		*dd = (i + mask) ^ mask;
	}


	inline void SetMinimum(
		__in double d1,
		__in double d2,
		__in double d3,
		__in double d4,
		__inout double *e
		)
	{
		if ( *e > d1 )
			*e = d1;
		if ( *e > d2 )
			*e= d2;
		if ( *e > d3 )
			*e = d3;
		if ( *e > d4 )
			*e = d4;
	}

	static const int shift = sizeof(int) * CHAR_BIT - 1;
};

