#ifndef PEA_PWR_BRANCHANDBOUND_H
#define PEA_PWR_BRANCHANDBOUND_H

#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include "./Utils/BranchAndBoundNode.h"
#include "../Matrix/Matrix.h"

class BranchAndBound {
private:
    int **inputMatrix = nullptr;
    int matrixSize = 0;

    int **rootMatrix = nullptr;

    BranchAndBoundNode *bestNodeFound = nullptr;

    std::vector<std::pair<int, int>> withBest;
    int distanceBest = 0;

public:
    BranchAndBound();

    virtual ~BranchAndBound();

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    static int reduceRows(int **matrix, int size);

    static int reduceColumns(int **matrix, int size);

    static int getMinimumDefined(int **matrix, int row, int column, int size);

    static pair<int **, int **> splitBranches(int **matrix, int size, int row, int column);

    static int updateMatrixLeft(int **matrix, int size, const std::vector<pair<int, int>> &with);

    static int updateMatrixRight(int **matrix, int size, int row, int column);

    static std::vector<std::pair<std::pair<int, int>, int>>
    addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::vector<pair<int, int>> &with);

    static vector<pair<int, int>> getAllRemainingEdges(int **matrix, int size);

    void solve(BranchAndBoundNode *root);

    static std::pair<int, std::pair<int, int>>
    chooseWorstCase(int **matrix, int size);
};


#endif //PEA_PWR_BRANCHANDBOUND_H
