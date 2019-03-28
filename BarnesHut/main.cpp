#include "DataGenerator.h"
#include "BHNode.h"
#include <iostream>

void main() {
	DataGenerator dg(ION_NUM, MY_DIM);
	//MY_TYPE aPoint[MY_DIM] = {3,3,3};
	//rawDataGenerator.GenerateAllAtAPoint(aPoint);
	dg.generateRandom();

	MY_TYPE tarPos[MY_DIM];

	BHNode<MY_TYPE, MY_DIM>* root = new BHNode<MY_TYPE, MY_DIM>
        (0,ION_NUM,dg._coordinates,dg._charges, dg._ionIndices,dg._minPos,dg._maxPos);
	
	MY_TYPE tarElectricField[MY_DIM] = {};
	root->CalculateElectricField(tarPos, tarElectricField);
	printArray<MY_TYPE>("BH", tarElectricField, MY_DIM);

	MY_TYPE tarElectricFieldBF[MY_DIM] = {};
	dg.calculateElectricFieldBF(tarPos, tarElectricFieldBF);
	printArray<MY_TYPE>("BF", tarElectricFieldBF, MY_DIM);
    delete root;
}