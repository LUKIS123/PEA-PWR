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
    int **memo = nullptr;
    int distanceBest = INT_MAX;
    std::vector<int> tour;
    int **prev = nullptr;
    int FINISHED_STATE;
    int START_NODE;

    DynamicProgramming() = default;

    virtual ~DynamicProgramming();

    void displayLatestResults();

    void mainFun(Matrix *matrix, int matrixSize);

    void solveTSP(int startVertex);

    static bool notIn(int element, int subset);

    static std::vector<int> combinations(int r, int n);

    static void combinations(int set, int at, int r, int n, std::vector<int> subsets);

    void solve(int start);

    int TSP(int i, int state);

    void clearData();
};


#endif //PEA_PWR_DYNAMICPROGRAMMING_H
