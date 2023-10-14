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

    int d;

public:
    bool *toBeVisited = nullptr;
    List *visited = nullptr;
    bool *alreadyVisited = nullptr;

    BruteForce(Matrix *matrix, int matrixSize);

    virtual ~BruteForce();

    void branch(bool *toBeVisitedVertices, int distance, int currentMinDist);

    int minDistanceToFinish();

    void mainFun();

    void TSP(int currentVertice, int startVertice, int &helperSum, List *verticesHeap, List *helperHeap);

    void test2();

    void tsp2(int start, int v, int dh, List *verticesHeap, List *helperHeap);

};


#endif //PEA_PWR_BRUTEFORCE_H
