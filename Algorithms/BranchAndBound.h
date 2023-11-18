#ifndef PEA_PWR_BRANCHANDBOUND_H
#define PEA_PWR_BRANCHANDBOUND_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include "./Utils/BranchAndBoundNode.h"
#include "../Matrix/Matrix.h"

class BranchAndBound {
private:
    int **inputMatrix = nullptr;
    int matrixSize = 0;
public:
    int distanceBest = INT_MAX;
    std::list<std::pair<int, int>> withBest;
    std::list<int> pathBest;
    std::list<int> pathCurrentNode;

    BranchAndBound() = default;

    virtual ~BranchAndBound() = default;

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    void solveTSP(BranchAndBoundNode *root);

    static int reduceRows(int **matrix, int size);

    static int reduceColumns(int **matrix, int size);

    static int getMinimumDefined(int **matrix, int row, int column, int size);

    static pair<int **, int **> splitBranches(int **matrix, int size, int row, int column);

    static int updateMatrixLeft(int **matrix, int size, const std::list<pair<int, int>> &with,
                                const std::list<std::list<int>> &subTours);

    static int updateMatrixRight(int **matrix, int size, int row, int column);

    std::list<std::pair<std::pair<int, int>, int>>
    addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::list<pair<int, int>> &with,
                                         bool &outSuccess);

    static std::list<pair<int, int>> getAllRemainingEdges(int **matrix, int size);

    static std::pair<int, std::pair<int, int>> chooseBestCaseEdge(int **matrix, int size, bool &outSuccess);

    bool tryMakePath(const std::list<pair<int, int>> &with, pair<int, int> firstPair, pair<int, int> secondPair);

    static std::list<std::list<int>>
    buildSubToursFromEdges(std::pair<int, int> edge, const list<std::list<int>> &subTours, int **matrix);

    static void blockLoopFormation(std::list<int> subPath, int **matrix);
};


#endif //PEA_PWR_BRANCHANDBOUND_H
