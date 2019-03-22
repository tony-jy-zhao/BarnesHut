#include "RawDataGenerator.h"
#include "BHNode.h"
#include <iostream>

void main() {
	int NumOfIons = NUMOFIONS_;
	RawDataGenerator rawDataGenerator(NumOfIons, MY_DIM);
	//MY_TYPE aPoint[MY_DIM] = {3,3,3};
	//rawDataGenerator.GenerateAllAtAPoint(aPoint);
	rawDataGenerator.GenerateRandom();

	MY_TYPE tarPos[MY_DIM] = { -12.5, 2.5, 2.5 };

	BHNode<MY_TYPE, MY_DIM>* root = new BHNode<MY_TYPE, MY_DIM>(0);
	root->ImportRawData(NumOfIons, rawDataGenerator._coords, rawDataGenerator._charges);

	MY_TYPE tarElectricField[MY_DIM] = {};
	root->CalculateElectricField(tarPos, tarElectricField);
	root->Reset();
	printOutArray<MY_TYPE>("No BF", tarElectricField, MY_DIM);

	MY_TYPE tarElectricFieldBF[MY_DIM] = {};
	rawDataGenerator.CalculateElectricFieldBF(tarPos, tarElectricFieldBF);
	printOutArray<MY_TYPE>("BF", tarElectricFieldBF, MY_DIM);
}
