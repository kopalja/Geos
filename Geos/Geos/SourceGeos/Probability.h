#pragma once
#include "ImageHandler.h"
#include "GMM.h"
#include "GaussianBlur.h"
#include "SymmetricalFilter.h"
#include "..\SourceSharpness\S3.h"


enum SegmentationType
{
	Interactive, Sharpness, Color, Salience  	
};

class Location
{
public:
	int x;
	int y;
};

class Probability
{
public:

	void GetProbabitily(
		__in SegmentationType segmentationType,
		__in const Image & rOrigin,
		__out double ** ppProbability,
		__in const Location * pForeground = nullptr,
		__in const Location * pBackground = nullptr
		);

private:

	void InteractiveProbability(
		__in const Image & rOrigin,
		__in const Location * pForeground,
		__in const Location * pBackground,
		__out double ** ppProbability
		);

	void SharpnessProbability(
		__in const Image & rOrigin,
		__out double ** ppProbability
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




};
