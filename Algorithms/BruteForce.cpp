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

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }

    distance = INT_MAX;
    int helperSum = 0;
    TSP(0, 0, helperSum, heap, helper);
    if (heap->getSize() != 0) {
        heap->displayFromFront();
        std::cout << distance << std::endl;
    } else {
        std::cout << "Lipa" << std::endl;
    }
}

void BruteForce::TSP(int currentVertice, int startVertice, int &helperSum, List *verticesHeap, List *helperHeap) {

    helperHeap->pushEnd(currentVertice);

    //k2
    if (helperHeap->getSize() == matrixSize && matrix->getMatrix()[startVertice][currentVertice] != -1) {
        //k4
        helperSum += matrix->getMatrix()[currentVertice][startVertice];
        if (helperSum < distance) {
            distance = helperSum;
            verticesHeap->copyOf(helperHeap);

        }
        //k8
        helperSum -= matrix->getMatrix()[currentVertice][startVertice];
        helperHeap->popEnd();
        return;
    }

    // k3
    if (matrix->getMatrix()[currentVertice][startVertice] == -1) {
        //k17
        helperHeap->popEnd();
        return;
    }

    //k10
    alreadyVisited[currentVertice] = true;
    for (int u = 0; u < matrixSize; u++) {
        //k12
        if (alreadyVisited[u] || matrix->getMatrix()[currentVertice][u] == -1) {
            continue;
        }
        helperSum += matrix->getMatrix()[currentVertice][u];
        TSP(u, startVertice, helperSum, verticesHeap, helperHeap);
        helperSum -= matrix->getMatrix()[currentVertice][u];
    }
    //k16
    alreadyVisited[currentVertice] = false;
    // k17
    helperHeap->popEnd();
}

void BruteForce::test2() {
    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }

    List *heap = new List();
    List *helper = new List();
    distance = INT_MAX;
    int dh = 0;
    int v0 = 0;
    tsp2(v0, v0, dh, heap, helper);

    heap->displayFromFront();
    std::cout << "Best route: " << d << std::endl;
}

void BruteForce::tsp2(int start, int v, int dh, List *verticesHeap, List *helperHeap) {

    int u;

    helperHeap->pushEnd(v);              // zapamiętujemy na stosie bieżący wierzchołek

    if (helperHeap->getSize() < matrixSize)                  // jeśli brak ścieżki Hamiltona, to jej szukamy
    {
        alreadyVisited[v] = true;          // Oznaczamy bieżący wierzchołek jako odwiedzony
        for (u = 0; u < matrixSize; u++)       // Przeglądamy sąsiadów wierzchołka v
            if (matrix->getMatrix()[v][u] != -1 && !alreadyVisited[u]) // Szukamy nieodwiedzonego jeszcze sąsiada
            {
                dh += matrix->getMatrix()[v][u];        // Dodajemy wagę krawędzi v-u do sumy
                tsp2(start, u, dh, verticesHeap,
                     helperHeap);                 // Rekurencyjnie wywołujemy szukanie cyklu Hamiltona
                dh -= matrix->getMatrix()[v][u];        // Usuwamy wagę krawędzi z sumy
            }
        alreadyVisited[v] = false;         // Zwalniamy bieżący wierzchołek
    } else if (matrix->getMatrix()[start][v] != -1)         // Jeśli znaleziona ścieżka jest cyklem Hamiltona
    {
        dh += matrix->getMatrix()[v][start];           // to sprawdzamy, czy ma najmniejszą sumę wag
        if (dh < d)                   // Jeśli tak,
        {
            d = dh;                      // To zapamiętujemy tę sumę
            verticesHeap->copyOf(helperHeap); // oraz kopiujemy stos Sh do S
        }
        dh -= matrix->getMatrix()[v][start];           // Usuwamy wagę krawędzi v-v0 z sumy
    }
    helperHeap->popEnd();                         // Usuwamy bieżący wierzchołek ze ścieżki
}
