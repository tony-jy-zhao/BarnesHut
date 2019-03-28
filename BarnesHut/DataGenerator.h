#pragma once
#include <vector>
#include "constants.h"

class DataGenerator
{
public:
	DataGenerator(int ionNum, int dim);
	~DataGenerator();
	void generateRandom();
	void generateAllAtAPoint(MY_TYPE* thePoint);
	void calculateElectricFieldBF(MY_TYPE* tarPos, MY_TYPE* tarElectricFieldBF);
	MY_TYPE* _coordinates;
	int* _charges;
    std::vector<int> _ionIndices;
    MY_TYPE _minPos[MY_DIM];
    MY_TYPE _maxPos[MY_DIM];

private:
	const int ionNum_;
	const int dim_;
};