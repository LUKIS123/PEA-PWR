#include "BruteForce.h"
#include <algorithm>

BruteForce::BruteForce(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->matrix = matrix;
    toBeVisited = new bool[matrixSize];
    visited = new List();
}

BruteForce::~BruteForce() {
    delete toBeVisited;
    delete visited;
}

void BruteForce::branch(bool *toBeVisitedVertices, int dist, int currentMinDist) {
    if (visited->getSize() == matrixSize) {

    } else {
        for (int i = 0; i < matrixSize; i++) {
            visited->pushEnd(i);
            if (visited->getSize() > 1) {
                dist += matrix->getMatrix()[visited->getByIndex(visited->getSize() - 2)->data]
                [visited->getByIndex(visited->getSize() - 1)->data];
            }
            if (dist > currentMinDist) {
                continue;
            }
            if (dist + minDistanceToFinish() > currentMinDist) {
                continue;
            }


        }
    }

}

int BruteForce::minDistanceToFinish() {
    int distanceToFinish = 0;
    for (int i = 0; i < matrixSize; i++) {
        int d = INT_MAX;
        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {
                continue;
            }
            d = std::min(d, matrix->getMatrix()[i][j]);
        }
        distanceToFinish += d;
    }
    return distanceToFinish;
}

void BruteForce::mainFun() {
    List *heap = new List();
    List *helper = new List();
    visited = new List(); // do wywalenia

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }

    TSP(matrixSize, 0, 0, INT_MAX, 0, heap, helper, visited);
    if (heap->getSize() == 0) {
        heap->displayFromFront();
    } else {
        std::cout << "Lipa" << std::endl;
    }
}

void BruteForce::TSP(int n_vertices, int currentVertice, int startVertice, int hamiltonCycleSum, int helperSum,
                     List *verticesHeap, List *helperHeap, List *visited) {
    helperHeap->pushEnd(currentVertice);

    if (matrix->getMatrix()[currentVertice][startVertice] == -1) {
        helperHeap->popEnd();
        return;
    }
    if (helperHeap->getSize() == matrixSize) {

        //k4
        helperSum += matrix->getMatrix()[currentVertice][startVertice];
        if (helperSum < hamiltonCycleSum) {
            hamiltonCycleSum = helperSum;
            verticesHeap = helperHeap;

        }
        //k8
        helperSum -= matrix->getMatrix()[currentVertice][startVertice];
        helperHeap->popEnd();
        return;
    }

    //k10
    alreadyVisited[currentVertice] = true;
    for (int u; u < matrixSize; u++) {
        //k12
        if (alreadyVisited[u]) {
            continue;
        }
        helperSum += matrix->getMatrix()[currentVertice][u];
        TSP(matrixSize, u, startVertice, hamiltonCycleSum, helperSum, verticesHeap, helperHeap, visited);
        helperSum -= matrix->getMatrix()[currentVertice][u];
    }

    //k16
    alreadyVisited[currentVertice] = false;

    // k17
    helperHeap->popEnd();
}


