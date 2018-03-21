#pragma once
#include "ImageHandler.h"
#include "GMM.h"
#include "GaussianBlur.h"
#include "SymmetricalFilter.h"
#include "..\SourceSharpness\S3.h"
#include "ExtendedFunctions.h"


enum SegmentationType
{
	Interactive, Sharpness, Color, Salience, SharpAndColor  	
};


class Probability
{
public:

	void GetProbabitily(
		__in SegmentationType segmentationType,
		__in const Image & rOrigin,
		__in const Image & rGrayImage,
		__in const vector<Location> & rForeground, 
		__in const vector<Location> & rBackground,
		__out double ** ppProbability
		);

private:

	void InteractiveProbability(
		__in const Image & rOrigin,
		__in const Image & rGrayImage,
		__in const vector<Location> & rForeground, 
		__in const vector<Location> & rBackground,
		__inout double ** ppProbability
		);

	void SharpnessProbability(
		__in const Image & rOrigin,
		__out double ** ppProbability,
		__in bool UseModifySharpness2 = false
		);

	/* Modify pixels probability surrounded by sharp pixels */
	void ModifySharpness1(
		__in const Image & rS3Image,
		__in const Image & rOrigin,
		__inout double ** ppProbability
		);

	/* Modify pixels probability of the same color as sharp object */
	void ModifySharpness2(
		__in const Image & rS3Image,
		__in const Image & rOrigin,
		__inout double ** ppProbability
		);


	void SalienceProbability(
		__in const Image & rOrigin,
		__out double ** ppProbability
		);

	void AverageColor(
		__in const Image & rOrigin,
		__out BYTE * red,
		__out BYTE * green,
		__out BYTE * blue
		);

	void InitDistance(
		__in int width,
		__in int height,
		__out double ** ppDistance
		);

	void SegmentInteractive(
		__in const Image & rOrigin,
		__in const Location * pSelectedPixel,
		__out double ** ppProbability
		);

	inline void SetSamples(
		__in int width,
		__in int height,
		__in double ** ppProbability,
		__out vector<Location> * pForeGround,
		__out vector<Location> * pBackGround
		)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if ( ppProbability[x][y] > 0.95 )
					pForeGround->push_back( Location( x, y ) );
				else if ( ppProbability[x][y] < 0.05 )
					pBackGround->push_back( Location( x, y ) );
			}
		}
	}

};
