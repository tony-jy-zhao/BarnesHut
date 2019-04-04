#include <iostream>
#include "IonsGenerator.h"
#include "BHNode.h"
#include "TimeDuration.h"

void main() {
	IonsGenerator ig(ION_NUM, MY_DIM);
	ig.generateRandom();

	int tarIon = rand() % ION_NUM;
	MY_TYPE tarPos[MY_DIM];
	for (int d = 0; d < MY_DIM; d++) {
		tarPos[d] = ig._coordinates[tarIon*MY_DIM + d];
	}

	double t1, t2;
	TimeDuration td(1);
	
	MY_TYPE reElectricFieldsBF[MY_DIM * ION_NUM];
	MY_TYPE reElectricFieldsBH[MY_DIM * ION_NUM];
	t1 = td.calculateTime([&]() {
		ig.calculateEveryElectricFieldsBF(reElectricFieldsBF);
		});

	t2 = td.calculateTime([&]() {
		BHNode<MY_TYPE, MY_DIM>* root = new BHNode<MY_TYPE, MY_DIM>
			(0, ION_NUM, ig._coordinates, ig._charges, ig._ionIndices, ig._minPos, ig._maxPos);

		for (int i = 0; i < ION_NUM; i++) {
			root->CalculateElectricField(i, &(reElectricFieldsBH[i*MY_DIM]));
		}
		delete root;
	});

	std::cout << t1 << std::endl << t2 << std::endl;

	// check calculationError
	for (int d = 0; d < MY_DIM; d++) {
//		fabs(tarElectricField[d] - tarElectricFieldBF[d]);
	}
}