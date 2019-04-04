#pragma once
#include <vector>
#include "constants.h"

class IonsGenerator
{
public:
	IonsGenerator(int ionNum, int dim);
	~IonsGenerator();
	void generateRandom();
	void generateAllAtAPoint(MY_TYPE* thePoint);
	void calculateElectricFieldBF(int tarIon, MY_TYPE* tarElectricFieldBF);
	void calculateEveryElectricFieldsBF(MY_TYPE* reEFieldVector);
	MY_TYPE* _coordinates;
	int* _charges;
    std::vector<int> _ionIndices;
    MY_TYPE _minPos[MY_DIM];
    MY_TYPE _maxPos[MY_DIM];

private:
	const int ionNum_;
	const int dim_;
};