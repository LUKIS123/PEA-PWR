#ifndef PEA_PWR_BRUTEFORCE_H
#define PEA_PWR_BRUTEFORCE_H

#include "iostream"
#include "Utils/List.h"
#include "../Matrix/Matrix.h"

class BruteForce {
private:
    Matrix *matrix = nullptr;
    int matrixSize = 0;
    int distance = 0;

public:
    bool *toBeVisited = nullptr;
    List *visited = nullptr;
    bool *alreadyVisited = nullptr;

    BruteForce(Matrix *matrix, int matrixSize);

    virtual ~BruteForce();

    void branch(bool *toBeVisitedVertices, int distance, int currentMinDist);

    int minDistanceToFinish();

    void mainFun();

    void
    TSP(int n_vertices, int currentVertice, int startVertice, int hamiltonCycleSum, int helperSum, List *verticesHeap,
        List *helperHeap, List *visited);

};


#endif //PEA_PWR_BRUTEFORCE_H
