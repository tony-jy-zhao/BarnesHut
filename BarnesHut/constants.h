#pragma once
#include <iostream>
#include <string>

#define MY_DIM 3
typedef float MY_TYPE;
const int NUMOFIONS_ = 2;
const int BOXSIDE_ = 50;

template <typename T>
inline void printOutArray(char* beginWords, T* arr, int arrayLength) {
	std::cout << beginWords << " ";
	for (int i = 0; i < arrayLength; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

/*
void SaveArrayToFile(int NumOfIons, void* coords)
{
	std::ofstream ofile("coordsFile.txt");
	MY_TYPE* co_ = (MY_TYPE*)coords;
	while (ofile.is_open())
	{
		for (size_t i = 0; i < NumOfIons; i++)
			for (size_t d = 0; d < MY_DIM; d++)
				ofile << co_[i*MY_DIM + d] << std::endl;
		ofile.close();
	}
}
*/