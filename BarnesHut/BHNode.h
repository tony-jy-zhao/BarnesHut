#pragma once
#include <vector>

// Barnes-Hut Tree Node
template <class DATA_TYPE, unsigned int DIM> class BHNode
{
public:
	unsigned int TotalCharge;
	DATA_TYPE[DIM] ChargeWeightedMeanPos;

	std::vector<int> IonsInNode;
	std::vector<BHNode*> ChildNodes;
	DATA_TYPE[DIM] MinPos;
	DATA_TYPE[DIM] MaxPos;
	bool HasChildren;
	unsigned int Depth;

	BHNode(unsigned int depth)
	{
		HasChildren = false;
		Depth = depth;
		TotalCharge = 0;
		for (int i = 0; i < DIM; i++)
			ChargeWeightedMeanPos[i] = 0;
	};
	~BHNode() {};

	void Reset() {
		IonsInNode.clear();
		for (int i = 0; i < ChildNodes.size(); i++)
			ChildNodes[i]->Reset();
		for (int i = 0; i < ChildNodes.size(); i++)
			delete ChildNodes[i];
		ChildNodes.clear();
		HasChildren = false;
	}

	DATA_TYPE[] CalculateElectricField(DATA_TYPE[] targetPos)
	{
	};

	void GenerateChildNodes()
	{
		unsigned int OrthantNum = 1;
		for (size_t i = 0; i < DIM; i++)
			OrthantNum *= 2;  //4 orthants in 2D; 8 orthants in 3D

		DATA_TYPE[DIM] MidPos;
		for (size_t i = 0; i < DIM; i++)
			MidPos[i] = (MinPos[i] + MaxPos[i])/2;

		auto dividedIons = std::vector<std::vector<unsigned int>>(OrthantNum);
		
		for (int i = 0; i < IonsInNode.size(); i++)
		{
			int ionIndex = IonsInNode[i];
			DATA_TYPE[DIM] ionPos = _IONS_[ionIndex]; // The casted array of ion positions from GPU
			int orthant = CalRelativeOrthant(ionPos, MidPos); // can use binary to implement, to get rid of dimension dependence		
			dividedIons[orthant].push_back(ionIndex);
		}

		for (int orthant = 0; orthant < OrthantNum; orthant++) {
			ChildNodes[orthant] = new BHNode(Depth + 1);
			DATA_TYPE[DIM] minPos = /* get minPos based on its orthant */;
			DATA_TYPE[DIM] maxPos = /* same as above */;
			ChildNodes[orthant]->SetParams(dividedIons[orthant], minPos, maxPos);
		}
	};

	void SetParams(std::vector<unsigned int>& ionsInNode, DATA_TYPE[] minPos, DATA_TYPE[] maxPos) {
		IonsInNode.swap(ionsInNode);
		DATA_TYPE[DIM] accPos;
		for (int i = 0; i < DIM; i++)
		{
			MinPos[i] = minPos[i];
			MaxPos[i] = maxPos[i];
			accPos[i] = 0;
		}
		TotalCharge = 0;
		
		for (int ionIndex: IonsInNode)
		{
			TotalCharge += _CHARGES_[ionIndex];
			for (unsigned int d = 0; d < DIM; d++)
				accPos[d] += _CHARGES_[ionIndex] * _IONS_[ionIndex][d];
		}
		
		for (size_t d = 0; d < DIM; d++)
		{
			ChargeWeightedMeanPos[d] = accPos[d] / TotalCharge;
		}

		if (IonsInNode.size() > 1 && Depth < /* Max Depth */)
			GenerateChildNodes();
	}
};

