#pragma once
#include "constants.h"

class RawDataGenerator
{
public:
	RawDataGenerator(int numOfIons, int dim);
	~RawDataGenerator();
	void GenerateRandom();
	void GenerateAllAtAPoint(MY_TYPE* thePoint);
	void CalculateElectricFieldBF(MY_TYPE* tarPos, MY_TYPE* tarElectricFieldBF);
	MY_TYPE* _coords;
	int* _charges;
private:
	int _numOfIons;
	int _dim;
};