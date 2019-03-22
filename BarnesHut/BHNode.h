#pragma once
#include <vector>
#include <numeric>

#define _THRESHOLD2_ 0.001
#define _MAXDEPTH_ 100

// Barnes-Hut Tree Node
template <class DATA_TYPE, unsigned int DIM>
class BHNode
{
public:
	BHNode(unsigned int depth)
	{
		HasChildren = false;
		Depth = depth;
		TotalCharge = 0;
		for (int i = 0; i < DIM; i++)
			ChargeWeightedMeanPos[i] = 0;

		OrthantNum = 1;
		for (size_t i = 0; i < DIM; i++)
			OrthantNum *= 2;  //4 orthants in 2D; 8 orthants in 3D
		ChildNodes = std::vector<BHNode*>(OrthantNum, nullptr);
	};

	~BHNode() {};

	void Reset() {
		IonsInNode.clear();
		for (int i = 0; i < ChildNodes.size(); i++) {
			if (ChildNodes[i] != nullptr)
			{
				ChildNodes[i]->Reset();
				delete ChildNodes[i];
			}
		}
		ChildNodes.clear();
		HasChildren = false;
	};

	// This should be the entry point of root
	void ImportRawData(unsigned int numOfIons, void* coords, void* charges) {
		// is it good practice to use pointer pointing to remote places
		NUMOFIONS_ = numOfIons;
		COORDS_ = (DATA_TYPE*)(coords);
		CHARGES_ = (int*)(charges);

		GenerateBoundary();

		std::vector<unsigned int> tmp(numOfIons);
		std::iota(std::begin(tmp), std::end(tmp), 0);
		SetParams(tmp, MinPos, MaxPos);
	};

	void CalculateElectricField(DATA_TYPE* targetPos, DATA_TYPE* efieldVector)
	{
		DATA_TYPE orthantSize2 = (MinPos[0] - MaxPos[0]) * (MinPos[0] - MaxPos[0]);
		DATA_TYPE distance2 = 0;
		DATA_TYPE distanceVector[DIM];

		for (size_t d = 0; d < DIM; d++) {
			distanceVector[d] = targetPos[d] - ChargeWeightedMeanPos[d];
			distance2 += distanceVector[d] * distanceVector[d];
		}
		if (orthantSize2 / distance2 < _THRESHOLD2_)	//TODO: _THRESHOLD2_ should be defined; // this is the case when the orthant is too far away
		{
			DATA_TYPE distance1 = sqrt(distance2);
			for (size_t d = 0; d < DIM; d++)
				efieldVector[d] += TotalCharge/distance2*distanceVector[d]/distance1;
		}
		else {
			for (BHNode* childNode: ChildNodes) {
				if (childNode != nullptr)
					childNode->CalculateElectricField(targetPos, efieldVector);
			}
		}
	};

	void GenerateChildNodes()
	{		
		DATA_TYPE MidPos[DIM];
		for (size_t d = 0; d < DIM; d++)
			MidPos[d] = (MinPos[d] + MaxPos[d])/2;

		auto dividedIons = std::vector<std::vector<unsigned int>>(OrthantNum);
		
		for (auto ionIndex: IonsInNode) {
			DATA_TYPE ionPos[DIM];
			for (size_t d = 0; d < DIM; d++)
				ionPos[d] = COORDS_[ionIndex * DIM + d];

			int orthant = CalRelativeOrthant(ionPos, MidPos); // can use binary to implement, to get rid of dimension dependence		
			dividedIons[orthant].emplace_back(ionIndex);
		}

		for (int orthant = 0; orthant < OrthantNum; orthant++) {
			ChildNodes[orthant] = new BHNode(Depth + 1);

			DATA_TYPE minPos[DIM];
			DATA_TYPE maxPos[DIM];
			CalculateMinMaxPos(orthant, minPos, maxPos);
			ChildNodes[orthant]->SetParams(dividedIons[orthant], minPos, maxPos);
		}
	};

	void SetParams(std::vector<unsigned int>& ionsInNode, DATA_TYPE* minPos, DATA_TYPE* maxPos) {
		IonsInNode.swap(ionsInNode);
		DATA_TYPE accPos[DIM];
		for (int i = 0; i < DIM; i++)
		{
			MinPos[i] = minPos[i];
			MaxPos[i] = maxPos[i];
			accPos[i] = 0;
		}
		TotalCharge = 0;
		
		for (int ionIndex: IonsInNode)
		{
			TotalCharge += CHARGES_[ionIndex];
			for (unsigned int d = 0; d < DIM; d++)
				accPos[d] += CHARGES_[ionIndex] * COORDS_[ionIndex*DIM+d];
		}
		
		for (size_t d = 0; d < DIM; d++)
			ChargeWeightedMeanPos[d] = accPos[d] / TotalCharge; // TODO: 0 check

		if (IonsInNode.size() > 1 && Depth < MAX_DEPTH)
			GenerateChildNodes();
	};
private:
	int CalRelativeOrthant(DATA_TYPE* target, DATA_TYPE* center) {
		int orthant = 0;
		for (size_t d = 0; d < DIM; d++) {
			orthant <<= 1;
			if (target[d] < center[d])
				orthant += 0;
			else
				orthant += 1;
		}
		return orthant;
	};

	void CalculateMinMaxPos(int orthant, DATA_TYPE* minPos, DATA_TYPE* maxPos) {
		for (int d = DIM-1; d >= 0; --d)
		{
			int cmp = orthant & 1;
			minPos[d] = cmp == 0 ? MinPos[d] : (MinPos[d] + MaxPos[d]) / 2;
			maxPos[d] = cmp == 0 ? (MinPos[d] + MaxPos[d]) / 2 : MaxPos[d];
			orthant >>= 1;
		}
	};

	void GenerateBoundary() {
		for (int d = 0; d < DIM; d++) {
			MinPos[d] = (MY_TYPE)INT_MAX;
			MaxPos[d] = (MY_TYPE)INT_MIN;
		}

		for (size_t i = 0; i < NUMOFIONS_; i++)
		{
			for (size_t d = 0; d < DIM; d++)
			{
				MinPos[d] = COORDS_[i*DIM + d] < MinPos[d] ? COORDS_[i*DIM + d] : MinPos[d];
				MaxPos[d] = COORDS_[i*DIM + d] > MaxPos[d] ? COORDS_[i*DIM + d] : MaxPos[d];
				// probably there is one ion just sit at the corner, which will cause problem.
				// but that's unlikely
			}
		}
	};

	unsigned int TotalCharge;
	DATA_TYPE ChargeWeightedMeanPos[DIM];
	std::vector<unsigned int> IonsInNode;
	std::vector<BHNode*> ChildNodes;
	DATA_TYPE MinPos[DIM];
	DATA_TYPE MaxPos[DIM];
	static DATA_TYPE* COORDS_;
	static int* CHARGES_;
	static int NUMOFIONS_;
	static int OrthantNum;
	bool HasChildren;
	unsigned int Depth;
	const int MAX_DEPTH = _MAXDEPTH_;
};

template <class DATA_TYPE, unsigned int DIM> DATA_TYPE* BHNode<DATA_TYPE, DIM>::COORDS_=NULL;
template <class DATA_TYPE, unsigned int DIM> int* BHNode<DATA_TYPE, DIM>::CHARGES_ = NULL;
template <class DATA_TYPE, unsigned int DIM> int BHNode<DATA_TYPE, DIM>::NUMOFIONS_ = 0;
template <class DATA_TYPE, unsigned int DIM> int BHNode<DATA_TYPE, DIM>::OrthantNum = 0;