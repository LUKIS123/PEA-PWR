#ifndef PEA_PWR_BRANCHANDBOUNDNODE_H
#define PEA_PWR_BRANCHANDBOUNDNODE_H

#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class BranchAndBoundNode {
public:
    int **data;
    int size;
    int location;

    int upperBound = 0;
    vector<pair<int, int>> with;
    vector<pair<int, int>> without;
    bool isKilled = false;

    BranchAndBoundNode(int **data, int size);

    BranchAndBoundNode(BranchAndBoundNode &node);

    virtual ~BranchAndBoundNode();

    bool operator<(const BranchAndBoundNode &other) const {
        return upperBound < other.upperBound;
    }

    bool operator>(const BranchAndBoundNode &other) const {
        return upperBound > other.upperBound;
    }

    class comp {
    public:
        bool operator()(const BranchAndBoundNode *lhs, const BranchAndBoundNode *rhs) const {
            return lhs->upperBound > rhs->upperBound;
        }
    };
};


#endif //PEA_PWR_BRANCHANDBOUNDNODE_H
