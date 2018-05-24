#pragma once
#include <Windows.h>
#include "ImageHandler.h"
#include "GMM.h"
#include "Probability.h"


/*
	Count energy after creating symmetrical mask with given diameter.
	All energies are couting parallel.
	Arguments are passed in ctor. Process starts after calling EntryPoint().
	Counted results are stored in variables "ppLabeling" and "value".
	"ppLabeling" contains final labeling and "value" counts energy 
	associated with given labeling. Method CountEnergy() is implemented
	after the Geos atricle equation (4).
*/


class Energy
{
public:
	/* Final results */
	bool ** ppLabeling;
	int value;

	Energy(
		__in int diameter,
		__in SymmetricalFilter * symmetricalFilter,
		__in double ** ppProbability,
		__in Image & rGrayImage
	) : rGrayImage(rGrayImage)
	{
		this->diameter = diameter;
		this->symmetricalFilter = symmetricalFilter;
		this->ppProbability = ppProbability;

		/* Aloc Labeling */
		ppLabeling = new bool*[rGrayImage.width];
		for (size_t i = 0; i < rGrayImage.width; i++)
			ppLabeling[i] = new bool[rGrayImage.height];
	}

	~Energy()
	{
		for (size_t i = 0; i < rGrayImage.width; i++)
		{
			delete[] ppLabeling[i];
		}
		delete[] ppLabeling;
	}

	void EntryPoint()
	{
		/* Count Labeling */
		symmetricalFilter->GetSymmetricalMask(diameter, diameter + 32, ppLabeling);

		/* Count energy of labeling */
		CountEnergy();
	}

private:
	int diameter;
	SymmetricalFilter * symmetricalFilter;
	double ** ppProbability;
	Image & rGrayImage;

	void CountEnergy()
	{
		double energyU = 0.0;
		double energyV = 0.0;
		double teta = 1.0;
		double n = 4.0;

		// Energy of U(z, a)
		for (int y = 0; y < rGrayImage.height; y++)
		{
			for (int x = 0; x < rGrayImage.width; x++)
			{
				if (ppLabeling[x][y] == true)
					energyU += ppProbability[x][y];
				else
					energyU += 1.0 - ppProbability[x][y];
			}
		}
		energyU *= -1.0;
		double temp;
		// Enegry of V(z, a)
		for (int y = 0; y < rGrayImage.height - 1; y++)
		{
			for (int x = 1; x < rGrayImage.width - 1; x++)
			{
				if (ppLabeling[x][y] != ppLabeling[x + 1][y])
					energyV -= expApproximation(-1 * (abs(rGrayImage.buffer[y * rGrayImage.width + x] - rGrayImage.buffer[y * rGrayImage.width + x + 1]) >> 2));
				if (ppLabeling[x][y] != ppLabeling[x - 1][y + 1])
					energyV -= expApproximation(-1 * (abs(rGrayImage.buffer[y * rGrayImage.width + x] - rGrayImage.buffer[(y + 1) * rGrayImage.width + x - 1]) >> 2));
				if (ppLabeling[x][y] != ppLabeling[x + 0][y + 1])
					energyV -= expApproximation(-1 * (abs(rGrayImage.buffer[y * rGrayImage.width + x] - rGrayImage.buffer[(y + 1) * rGrayImage.width + x + 0]) >> 2));
				if (ppLabeling[x][y] != ppLabeling[x + 1][y + 1])
					energyV -= expApproximation(-1 * (abs(rGrayImage.buffer[y * rGrayImage.width + x] - rGrayImage.buffer[(y + 1) * rGrayImage.width + x + 1]) >> 2));
			}
		}
		energyV *= -1;


		value = energyU + teta * energyV;
	}

};

