#ifndef PEA_PWR_BRUTEFORCE_H
#define PEA_PWR_BRUTEFORCE_H

#include "iostream"
#include "Utils/List.h"
#include "../Matrix/Matrix.h"

class BruteForce {
private:
    int **matrix = nullptr;
    int matrixSize = 0;
    int distance = 0;

    List *stack;
    List *temporaryStack;

    int d;

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

    void test2(Matrix *matrix, int matrixSize);

    void tsp2(int start, int v, int dh, List *verticesHeap, List *helperHeap);
};


#endif //PEA_PWR_BRUTEFORCE_H
