
#include <Windows.h>
#include "ImageHandler.h"




class SymmetricalFilter
{
public:

	SymmetricalFilter(
		__in double alfa,
		__in const Image *pGrayImage, 
		__in const double **ppInputProbability
		);

	SymmetricalFilter( double alfa ){ m_Alfa = alfa; m_NeedToDealoc = false; }

	~SymmetricalFilter(
		);

	void GetSymmetricalMask(
		__in int erosion, 
		__in int dilation, 
		__out bool **ppSymmetricalMask 
		);

	void CountUnSignedDistance(
		__in const Image *pGrayImage, 
		__inout double **ppDistance,
		__in bool normalDistance = true
		);

	double **m_ppSignedDistance;

private:
	double m_Alfa;
	bool m_NeedToDealoc;
	UINT m_Width, m_Height;
	const Image *m_pGrayImage;


	void CountSignedDistance(
		__in const Image *pGrayImage, 
		__in const double **ppInputProbability
		);


	void InitDistance(
		__in bool toComplement,
		__in const double **ppInputProbability,
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

	inline void GrayBaseNeighborDistancesSharpArea(
		__in const BYTE *pBuffer,
		__in int index,
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
		*da = max( 0, pBuffer[index + dxa] - pBuffer[index] );
		*db = max( 0, pBuffer[index + dxb] - pBuffer[index] );
		*dc = max( 0, pBuffer[index + dxc] - pBuffer[index] );
		*dd = max( 0, pBuffer[index + dxd] - pBuffer[index] );

		*da *= *da * *da;
		*db *= *db * *db;
		*dc *= *dc * *dc;
		*dd *= *dd * *dd;
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
			*e = d2;
		if ( *e > d3 )
			*e = d3;
		if ( *e > d4 )
			*e = d4;
	}

	static const int shift = sizeof(int) * CHAR_BIT - 1;
};

