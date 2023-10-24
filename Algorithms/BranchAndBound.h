#ifndef PEA_PWR_BRANCHANDBOUND_H
#define PEA_PWR_BRANCHANDBOUND_H

#include "iostream"
#include "algorithm"
//#include <bits/stdc++.h>
#include "vector"
#include "./Utils/BranchAndBoundNode.h"
#include "../Matrix/Matrix.h"

class BranchAndBound {
private:
    int **inputMatrix = nullptr;
    int matrixSize = 0;
    int distance = 0;

    int **reducedMatrix = nullptr;

    std::vector<BranchAndBoundNode *> *nodeList = nullptr;
    BranchAndBoundNode *currentNode = nullptr;

public:
    BranchAndBound();

    virtual ~BranchAndBound();

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    int reduceRows(int **matrix, int size);

    int reduceColumns(int **matrix, int size);

    std::pair<int, std::pair<int, int>> chooseWorstCase(int **matrix, int size);

    int getMinimumDefined(int **matrix, int row, int column, int size);

    std::pair<int **, int **> splitBranches(int **matrix, int size, int row, int column);

    void branchOut();

    BranchAndBoundNode *getLowestBoundNode();
};


#endif //PEA_PWR_BRANCHANDBOUND_H
