#ifndef PEA_PWR_BRUTEFORCE_H
#define PEA_PWR_BRUTEFORCE_H

#include "iostream"
#include "Utils/List.h"
#include "../Matrix/Matrix.h"

class BruteForce {
private:
    int **matrix = nullptr;
    int matrixSize = 0;
    int currentBestDistance = 0;

    List *stack;
    List *temporaryStack;

public:
    bool *toBeVisited = nullptr;
    List *visited = nullptr;
    bool *alreadyVisited = nullptr;

    BruteForce();

    virtual ~BruteForce();

    void clearData();

    void displayLatestResults();

    int minDistanceToFinish(bool *visited);

    void mainFun(Matrix *matrix, int matrixSize, bool runOptimized);

    void TSP_OPT(int currentVertex, int startVertex, int &helperSum);

    void TSP(int currentVertex, int startVertex, int &helperSum);
};


#endif //PEA_PWR_BRUTEFORCE_H
