#ifndef PEA_PWR_BRANCHANDBOUND_H
#define PEA_PWR_BRANCHANDBOUND_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "./Utils/BranchAndBoundNode.h"
#include "../Matrix/Matrix.h"

class BranchAndBound {
private:
    int **inputMatrix = nullptr;
    int matrixSize = 0;
public:
    std::vector<std::pair<int, int>> withBest;
    int distanceBest = INT_MAX;
    std::vector<int> pathBest;
    std::vector<int> pathCurrentNode;
    int vertices = 0;

    BranchAndBound() = default;

    virtual ~BranchAndBound() = default;

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    void solveTSP(BranchAndBoundNode *root);

    static int reduceRows(int **matrix, int size);

    static int reduceColumns(int **matrix, int size);

    static int getMinimumDefined(int **matrix, int row, int column, int size);

    static pair<int **, int **> splitBranches(int **matrix, int size, int row, int column);

    static int updateMatrixLeft(int **matrix, int size, const std::vector<pair<int, int>> &with);

    static int updateMatrixRight(int **matrix, int size, int row, int column);

    std::vector<std::pair<std::pair<int, int>, int>>
    addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::vector<pair<int, int>> &with,
                                         bool &outSuccess);

    static vector<pair<int, int>> getAllRemainingEdges(int **matrix, int size);

    static std::pair<int, std::pair<int, int>> chooseBestCaseEdge(int **matrix, int size, bool &outSuccess);

    bool tryMakePath(const vector<pair<int, int>> &with, pair<int, int> firstPair, pair<int, int> secondPair);

    static bool checkForLoops(const vector<pair<int, int>> &with);
};


#endif //PEA_PWR_BRANCHANDBOUND_H
