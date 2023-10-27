#ifndef PEA_PWR_BRANCHANDBOUNDNODE_H
#define PEA_PWR_BRANCHANDBOUNDNODE_H

#include "iostream"
#include "vector"
//#include "bits/stdc++.h"

using namespace std;

class BranchAndBoundNode {
public:
    int **data;
    int size;
    int location;
    BranchAndBoundNode *parent = nullptr;

    // todo test
    bool branchedOut;
    int remainingVertices;
    int upperBound = 0;
    vector<pair<int, int>> path;
    vector<pair<int, int>> with;
    vector<pair<int, int>> without;

    BranchAndBoundNode(int **data, int size);

    virtual ~BranchAndBoundNode();

    bool operator<(const BranchAndBoundNode &other) const {
        if (branchedOut) return false;
        return upperBound < other.upperBound;
    }


    BranchAndBoundNode(const BranchAndBoundNode &node);

    BranchAndBoundNode(int **data, int size, int location, BranchAndBoundNode *parent, bool branchedOut,
                       int remainingVertices, int upperBound, const vector<pair<int, int>> &path,
                       const vector<pair<int, int>> &with, const vector<pair<int, int>> &without);
};


#endif //PEA_PWR_BRANCHANDBOUNDNODE_H
