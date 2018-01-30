#include "ImageHandler.h"
#include "S1.h"
#include "S2.h"

enum ResultType
{
	S1Image, S2Image, S3Image
};

#pragma once
class _declspec(dllexport) S3
{
public:
	S3( void );
	~S3( void );

	void Process( 
		__in char* inputPath, 
		__in char* outputPath, 
		__in int resultType = 3,
		__in int fourierReduction = 2, 
		__in bool skin = true 
		);

	void GrayScale( 
		__in const Image & rOrigin,
		__out Image * pGrayImage
		);

	void GrayToResult(
		__in ResultType resultType,
		__in int fourierReduction, 
		__in bool skin,
		__in const Image & pGrayImage,
		__out Image *pResultImage
		);

private:

	ResultType m_resultType;

	HRESULT ReadParams( 
		__in int resultType
	);


	void GetGrayScaleSize( 
		__in UINT imageWidth,
		__in UINT imageHeight,
		__out UINT *pWidth,
		__out UINT *pHeight,
		__out UINT *pSize
		);

	void Merge( 
		__in Image *pTempImage, 
		__inout Image *pResultImage
		);
};

