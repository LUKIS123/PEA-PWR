#ifndef PEA_PWR_DYNAMICPROGRAMMING_H
#define PEA_PWR_DYNAMICPROGRAMMING_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "../Matrix/Matrix.h"

class DynamicProgramming {
private:
    int **inputMatrix = nullptr;
    int matrixSize = 0;
public:
    int **cache = nullptr;
    int distanceBest = INT_MAX;
    std::vector<int> path;
    int **previous = nullptr;
    int FINISHED_STATE = NULL;
    int START_NODE = NULL;

    DynamicProgramming() = default;

    virtual ~DynamicProgramming();

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    void solve(int start);

    int TSP(int currentVertex, int currentState);

    void clearData();
};


#endif //PEA_PWR_DYNAMICPROGRAMMING_H
