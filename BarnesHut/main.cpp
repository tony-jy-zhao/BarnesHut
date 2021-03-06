#include <iostream>
#include <fstream>
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
	
	MY_TYPE reElectricFieldsBF[MY_DIM * ION_NUM] = {0};
	MY_TYPE reElectricFieldsBH[MY_DIM * ION_NUM] = {0};
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
	std::ofstream myFile("diffBtwTwo.txt");
	std::ofstream devFile("dev.txt");
	if (myFile.is_open() && devFile.is_open()) {
		for (int i = 0; i < MY_DIM * ION_NUM; i++) {
			MY_TYPE x1 = reElectricFieldsBF[i];
			MY_TYPE x2 = reElectricFieldsBH[i];
			MY_TYPE dev = 2 * (x1 - x2) / (x1 + x2);
			myFile << dev << "\n";
			if (dev > 0.1) {
				devFile << dev << "\t" << x1 << "\t" << x2 << "\n";
			}
		}
		myFile.close();
		devFile.close();
	}
}