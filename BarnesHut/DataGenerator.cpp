#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <fstream>
#include <iostream>

#include "DataGenerator.h"

DataGenerator::DataGenerator(int ionNum, int dim):
    dim_(dim),
    ionNum_(ionNum)
{
	_coordinates = (MY_TYPE*)malloc(ionNum_ * dim_ * sizeof(MY_TYPE));
	_charges = (int*)malloc(ionNum_ * sizeof(int));
	std::fill(_charges, _charges+ionNum_, 1);

    _ionIndices=std::vector<int>(ionNum_);
    std::iota(std::begin(_ionIndices), std::end(_ionIndices), 0);
}

DataGenerator::~DataGenerator()
{
	free(_coordinates);
	free(_charges);
}

void DataGenerator::generateRandom()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < ionNum_ * MY_DIM; i++) {
		_coordinates[i] = static_cast<MY_TYPE>(rand()) / (static_cast<MY_TYPE>(RAND_MAX/BOX_SIDE_LENGTH));
	}
    for (int d = 0; d < dim_; d++) {
        _minPos[d] = 0;
        _maxPos[d] = BOX_SIDE_LENGTH;
    }
}

void DataGenerator::generateAllAtAPoint(MY_TYPE* thePoint)
{
	for (int i = 0; i < ionNum_; i++) {
		for (int d = 0; d < dim_; d++) {
			_coordinates[i*dim_ + d] = thePoint[d];
		}
	}
}

void DataGenerator::calculateElectricFieldBF(MY_TYPE* tarPos, MY_TYPE* tarElectricFieldBF)
{
    for (int i = 0; i < ionNum_; i++) {
        MY_TYPE distance2 = 0;
        for (int d = 0; d < MY_DIM; d++) {
            distance2 += (tarPos[d] - _coordinates[i*MY_DIM + d]) * (tarPos[d] - _coordinates[i*MY_DIM + d]);
        }
        MY_TYPE distance = sqrt(distance2);
        for (size_t d = 0; d < MY_DIM; d++) {
            tarElectricFieldBF[d] += _charges[i] / distance2 * (tarPos[d] - _coordinates[i*MY_DIM + d]) / distance;
        }
    }
}