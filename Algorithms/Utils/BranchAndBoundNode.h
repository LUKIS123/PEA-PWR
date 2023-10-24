#ifndef PEA_PWR_BRANCHANDBOUNDNODE_H
#define PEA_PWR_BRANCHANDBOUNDNODE_H

#include "iostream"
#include "vector"
//#include "bits/stdc++.h"

using namespace std;

class BranchAndBoundNode {
public:
    int **data;
    //int rem;
    int weight;
    int size;
    vector<bool> visited;
    int location;
    BranchAndBoundNode *parent = nullptr;

    // todo test
    int upperBound = 0;
    int *vertices = nullptr;
    vector<int> with;
    vector<int> without;

    BranchAndBoundNode(int **data, int size);

    virtual ~BranchAndBoundNode();
};


#endif //PEA_PWR_BRANCHANDBOUNDNODE_H
