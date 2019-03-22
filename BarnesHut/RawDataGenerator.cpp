#include "RawDataGenerator.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include <algorithm>
#include <fstream>
#include <iostream>

void RawDataGenerator::CalculateElectricFieldBF(MY_TYPE* tarPos, MY_TYPE* tarElectricFieldBF)
{
	for (int i = 0; i < _numOfIons; i++) {
		MY_TYPE distance2 = 0;
		for (int d = 0; d < MY_DIM; d++) {
			distance2 += (tarPos[d] - _coords[i*MY_DIM + d]) * (tarPos[d] - _coords[i*MY_DIM + d]);
		}
		MY_TYPE distance = sqrt(distance2);
		for (size_t d = 0; d < MY_DIM; d++) {
			tarElectricFieldBF[d]+=_charges[i]/distance2*(tarPos[d]-_coords[i*MY_DIM+d])/distance;
		}
	}
}

RawDataGenerator::RawDataGenerator(int numOfIons, int dim)
{
	_dim = dim;
	_numOfIons = numOfIons;
	_coords = (MY_TYPE*)malloc(numOfIons * dim * sizeof(MY_TYPE));
	_charges = (int*)malloc(numOfIons * sizeof(int));
	std::fill(_charges, _charges + numOfIons, 1);
}

RawDataGenerator::~RawDataGenerator()
{
	free(_coords);
	free(_charges);
}

void RawDataGenerator::GenerateRandom()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < _numOfIons * MY_DIM; i++) {
		_coords[i] = static_cast<MY_TYPE>(rand()) / (static_cast<MY_TYPE>(RAND_MAX/BOXSIDE_));
	}
}

void RawDataGenerator::GenerateAllAtAPoint(MY_TYPE* thePoint)
{
	for (int i = 0; i < _numOfIons; i++)
	{
		for (int d = 0; d < _dim; d++)
		{
			_coords[i*_dim + d] = thePoint[d];
		}
	}
}


