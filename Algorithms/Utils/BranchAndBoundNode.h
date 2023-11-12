#ifndef PEA_PWR_BRANCHANDBOUNDNODE_H
#define PEA_PWR_BRANCHANDBOUNDNODE_H

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class BranchAndBoundNode {
public:
    int **data;
    int size;
    int location;

    int lowerBound = 0;
    vector<pair<int, int>> with;
    vector<pair<int, int>> without;
    vector<list < int>> subTours;

    BranchAndBoundNode(int **data, int size);

    BranchAndBoundNode(BranchAndBoundNode &node);

    virtual ~BranchAndBoundNode();

    bool operator<(const BranchAndBoundNode &other) const {
        return lowerBound < other.lowerBound;
    }

    bool operator>(const BranchAndBoundNode &other) const {
        return lowerBound > other.lowerBound;
    }

    class comp {
    public:
        bool operator()(const BranchAndBoundNode *lhs, const BranchAndBoundNode *rhs) const {
            return lhs->lowerBound > rhs->lowerBound;
        }
    };
};


#endif //PEA_PWR_BRANCHANDBOUNDNODE_H
