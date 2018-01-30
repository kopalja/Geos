#pragma once

#include "ImageHandler.h"
#include "Block.h"


class S2
{
public:

	void CreateS2( 
		__in const Image & pGrayImage,
		__out Image* pS2Image 
		);
private:

	void CountFirstRow( Block* rowToCount, const Image & image );
	inline void CountRow( Block* rowToCount, Block* rowAbove, const Image & image, int y );
	inline void SetBuffer( Block* upRow, Block* downRow, BYTE* buffer, int y, int height );

	int numberOfBlocks;

};

