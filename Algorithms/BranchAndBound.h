#ifndef PEA_PWR_BRANCHANDBOUND_H
#define PEA_PWR_BRANCHANDBOUND_H

#include "iostream"
#include "algorithm"
#include <stack>
//#include <bits/stdc++.h>
#include "vector"
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

    std::vector<BranchAndBoundNode *> *nodeList = nullptr;
    BranchAndBoundNode *currentNode = nullptr;

    std::vector<int> *currentPathVertices = nullptr;

public:
    BranchAndBound();

    virtual ~BranchAndBound();

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    static int reduceRows(int **matrix, int size);

    static int reduceColumns(int **matrix, int size);

    static std::pair<int, std::pair<int, int>> chooseWorstCase(int **matrix, int size);

    static int getMinimumDefined(int **matrix, int row, int column, int size);

    void branchOut();

    BranchAndBoundNode *getLowestBoundNode();

    static pair<int **, int **> splitBranches(int **matrix, int size, int row, int column);

    static int updateMatrixLeft(int **matrix, int size, const std::vector<pair<int, int>> &with);

    static int updateMatrixRight(int **matrix, int size, int row, int column);

    std::vector<std::pair<std::pair<int, int>, int>> addRemainingEdges(int **matrix);

    pair<vector<std::pair<int, int>>, vector<std::pair<int, int>>> checkConnections(
            const std::vector<std::pair<int, int>> &with);

    vector<pair<int, int>> getRemainingEdges(int **matrix, int size);

    void solve(BranchAndBoundNode *root);
};


#endif //PEA_PWR_BRANCHANDBOUND_H
