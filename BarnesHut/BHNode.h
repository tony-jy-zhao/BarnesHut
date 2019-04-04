#pragma once
#include <vector>
#include <assert.h>

#define THRESHOLD2 (-1)
#define MAX_DEPTH (INT_MAX)

// Barnes-Hut Tree Node
template <class T, int D>
class BHNode
{
public:
    BHNode(int depth, const int allIonNum, const T* coordinates, const int* charges, std::vector<int>& ionsInNode, T* minPos, T* maxPos): 
        c_orthantNum_([]()->int { int orth = 1;
            for (int d = 0; d < D; d++) orth *= 2; return orth; }()),
        c_depth_(depth),
        totChargeInNode_(0),
        childNodes_(std::vector<BHNode*>(this->c_orthantNum_, nullptr)),
        c_allIonNum_(allIonNum),
        c_allCoordinates_(coordinates),
        c_allCharges_(charges),
        ionsInNode_(ionsInNode) {
#ifdef BHNODE_DEBUG
            for (int d = 0; d < D; d++) {
                assert(chargeWeightedMeanPos_[d] == 0);
        }
            if (D == 2) {
                assert(orthantNum_ == 4);
            }
            else if (D == 3) {
                assert(orthantNum_ == 8);
            }
#endif // BHNODE_DEBUG

        T accPos[D] = {0};
        for (int d = 0; d < D; d++)
        {
            chargeWeightedMeanPos_[d] = 0;
            minPos_[d] = minPos[d];
            maxPos_[d] = maxPos[d];
        }

        for (int ionIndex : ionsInNode_)
        {
            totChargeInNode_ += c_allCharges_[ionIndex];
            for (int d = 0; d < D; d++) {
                accPos[d] += c_allCharges_[ionIndex] * c_allCoordinates_[ionIndex*D + d];
            }
        }

#ifdef BHNODE_DEBUG
        assert(totChargeInNode_ > 0);
#endif // BHNODE_DEBUG
        for (int d = 0; d < D; d++) {
            chargeWeightedMeanPos_[d] = accPos[d] / totChargeInNode_; // TODO: 0 check
        }
        if (ionsInNode_.size() > 1 && c_depth_ < MAX_DEPTH) {
            GenerateChildNodes();
        }
	};

	~BHNode()
    {
        for (auto cn: childNodes_) {
            if (cn != nullptr) {
                delete cn;
            }
        }
    };

    void GenerateChildNodes()
    {
        T midPos[D] = {0};
        for (int d = 0; d < D; d++) {
            midPos[d] = (minPos_[d] + maxPos_[d]) / 2;
        }

        auto dividedIons = std::vector<std::vector<int>>(c_orthantNum_);

        for (auto ionIndex : ionsInNode_) {
            T ionPos[D];
            for (int d = 0; d < D; d++) {
                ionPos[d] = c_allCoordinates_[ionIndex * D + d];
            }

            int orthant = calculateOrthant(ionPos, midPos);
            dividedIons[orthant].emplace_back(ionIndex);
        }

        for (int orthant = 0; orthant < c_orthantNum_; orthant++) {
            if (dividedIons[orthant].size()>0)
            {
                T minPos[D];
                T maxPos[D];
                calculateMinMaxPos(orthant, minPos, maxPos);
                childNodes_[orthant] = new BHNode(c_depth_+1, c_allIonNum_, c_allCoordinates_, c_allCharges_, dividedIons[orthant], minPos, maxPos);
            }
        }
    };

	void CalculateElectricField(int tarIon, T* efieldVector) {
		const T* targetPos = &(c_allCoordinates_[tarIon*MY_DIM]);

		T orthantSize2 = (minPos_[0] - maxPos_[0]) * (minPos_[0] - maxPos_[0]);
		T distance2 = 0;
		T distanceVector[D];

		for (int d = 0; d < D; d++) {
			distanceVector[d] = targetPos[d] - chargeWeightedMeanPos_[d];
			distance2 += distanceVector[d] * distanceVector[d];
		}

		if (distance2 == 0) {
			return;
		}

		if (ionsInNode_.size() == 1 || orthantSize2 / distance2 < THRESHOLD2) {
			T distance1 = sqrt(distance2);
			for (int d = 0; d < D; d++) {
				efieldVector[d] += totChargeInNode_ / distance2 * distanceVector[d] / distance1;
			}
		}
		else {
			for (BHNode* childNode : childNodes_) {
				if (childNode != nullptr) {
					childNode->CalculateElectricField(tarIon, efieldVector);
				}
			}
		}
	};

    void PrintBHTree() {
        using namespace std;
        cout << "Depth=" << c_depth_ << " ";
        cout << "TotalCharge=" << totChargeInNode_ << " ";
        cout << "HasChildren=" << HasChildren << " ";
        printArray("MinPos=", minPos_, D);
        printArray("MaxPos=", maxPos_, D);
        printArray("ChargeWeightedMeanPos=", chargeWeightedMeanPos_, D);
        printArray("IonsInNode=", ionsInNode_, ionsInNode_.size());
        for (BHNode* childNode : childNodes_)
        {
            if (childNode != nullptr) {
                childNode->PrintBHTree();
            }
        }
    }
private:
	int calculateOrthant(T* target, T* center) {
		int orthant = 0;
        for (size_t d = 0; d < D; d++) {
            orthant <<= 1;
            if (target[d] < center[d]) {
                orthant += 0;
            }
            else {
                orthant += 1;
            }
        }
#ifdef BHNODE_DEBUG
        assert(orthant >= 0 && orthant < c_orthantNum_);
#endif // BHNODE_DEBUG

		return orthant;
	};

	void calculateMinMaxPos(int orthant, T* minPos, T* maxPos) {
		for (int d = D-1; d >= 0; --d)
		{
			int cmp = orthant & 1;
			minPos[d] = cmp == 0 ? minPos_[d] : (minPos_[d] + maxPos_[d]) / 2;
			maxPos[d] = cmp == 0 ? (minPos_[d] + maxPos_[d]) / 2 : maxPos_[d];
			orthant >>= 1;
		}
	};

	void generateBoundary() {
		for (int d = 0; d < D; d++) {
			minPos_[d] = (MY_TYPE)BOX_SIDE_LENGTH;
			maxPos_[d] = (MY_TYPE)(-1);
		}

		for (size_t i = 0; i < c_allIonNum_; i++)
		{
			for (size_t d = 0; d < D; d++)
			{
				minPos_[d] = c_allCoordinates_[i*D + d] < minPos_[d] ? c_allCoordinates_[i*D + d] : minPos_[d];
				maxPos_[d] = c_allCoordinates_[i*D + d] > maxPos_[d] ? c_allCoordinates_[i*D + d] : maxPos_[d];
			}
		}
	};

    const int c_orthantNum_;
    const int c_allIonNum_;
    const T* c_allCoordinates_;
    const int* c_allCharges_;
    const int c_depth_;

	int totChargeInNode_;
	T chargeWeightedMeanPos_[D];
	std::vector<int> ionsInNode_;
	std::vector<BHNode*> childNodes_;
	T minPos_[D];
	T maxPos_[D];
};


// if coordinates_ is static, then the following line should be added
//template <class T, unsigned int D> T* BHNode<T, D>::coordinates_ = NULL;