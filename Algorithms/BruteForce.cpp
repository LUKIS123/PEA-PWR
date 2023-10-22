#include "BruteForce.h"
#include <algorithm>

BruteForce::BruteForce() {
    toBeVisited = new bool[matrixSize];
    visited = new List();

    stack = new List();
    temporaryStack = new List();
}

BruteForce::~BruteForce() {
    delete toBeVisited;
    delete visited;
}

void BruteForce::clearData() {
    if (stack->getSize() != 0) {
        stack->removeAll();
    }
    if (temporaryStack->getSize() != 0) {
        temporaryStack->removeAll();
    }
    delete alreadyVisited;
}

void BruteForce::displayLatestResults() {
    std::cout << "Path:" << std::endl;
    stack->displayFromFront();
    std::cout << "Distance: " << distance << std::endl;
}

void BruteForce::mainFun(Matrix *matrix, int matrixSize, bool runOptimized) {
    this->matrixSize = matrixSize;
    this->matrix = matrix->getMatrix();

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }
    distance = INT_MAX;
    int helperSum = 0;

    if (runOptimized) {
        TSP_OPT(0, 0, helperSum);
    } else {
        TSP(0, 0, helperSum);
    }
}

void BruteForce::TSP_OPT(int currentVertex, int startVertex, int &helperSum) {
    temporaryStack->pushEnd(currentVertex);
    //k2
    if (temporaryStack->getSize() == matrixSize && matrix[startVertex][currentVertex] != -1) {
        //k4
        helperSum += matrix[currentVertex][startVertex];
        if (helperSum < distance) {
            distance = helperSum;
            stack->copyOf(temporaryStack);
            stack->pushEnd(startVertex);
        }
        //k8
        helperSum -= matrix[currentVertex][startVertex];
        temporaryStack->popEnd();
        return;
    }
    // k3 => TO JEST CHYBA NIEPOTRZEBNE
    if (matrix[currentVertex][startVertex] == -1) {
        //k17
        if (currentVertex != startVertex) {
            temporaryStack->popEnd();
            return;
        }
    }
    //k10
    alreadyVisited[currentVertex] = true;
    for (int u = 0; u < matrixSize; u++) {
        //k12
        if (alreadyVisited[u] || matrix[currentVertex][u] == -1) {
            continue;
        }
        helperSum += matrix[currentVertex][u];

        // optymalizacja : There is no added value in extending a path that is already above the current minimal distance
        if (helperSum > distance) {
            helperSum -= matrix[currentVertex][u];
            continue;
        }
        // opt 2 : Further improvement would be to keep track of the minimal distance you would need to add in order to finish the current path
        if ((helperSum + minDistanceToFinish(alreadyVisited)) > distance) {
            helperSum -= matrix[currentVertex][u];
            continue;
        }
        // optymalizacja

        TSP_OPT(u, startVertex, helperSum);
        helperSum -= matrix[currentVertex][u];
    }
    //k16
    alreadyVisited[currentVertex] = false;
    // k17
    temporaryStack->popEnd();
}

int BruteForce::minDistanceToFinish(bool *visited) {
    int distanceToFinish = 0;
    for (int i = 0; i < matrixSize; i++) {
        int dist = INT_MAX;
        if (visited[i]) {
            continue;
        }

        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {
                continue;
            }
            dist = std::min(dist, matrix[i][j]);
        }

        distanceToFinish += dist;
    }
    return distanceToFinish;
}

void BruteForce::TSP(int currentVertex, int startVertex, int &helperSum) {
    temporaryStack->pushEnd(currentVertex);
    //k2
    if (temporaryStack->getSize() == matrixSize && matrix[startVertex][currentVertex] != -1) {
        //k4
        helperSum += matrix[currentVertex][startVertex];
        if (helperSum < distance) {
            distance = helperSum;
            stack->copyOf(temporaryStack);
            stack->pushEnd(startVertex);
        }
        //k8
        helperSum -= matrix[currentVertex][startVertex];
        temporaryStack->popEnd();
        return;
    }
    // k3 => TO JEST CHYBA NIEPOTRZEBNE
    if (matrix[currentVertex][startVertex] == -1) {
        //k17
        if (currentVertex != startVertex) {
            temporaryStack->popEnd();
            return;
        }
    }
    //k10
    alreadyVisited[currentVertex] = true;
    for (int u = 0; u < matrixSize; u++) {
        //k12
        if (alreadyVisited[u] || matrix[currentVertex][u] == -1) {
            continue;
        }
        helperSum += matrix[currentVertex][u];

        TSP(u, startVertex, helperSum);

        helperSum -= matrix[currentVertex][u];
    }
    //k16
    alreadyVisited[currentVertex] = false;
    // k17
    temporaryStack->popEnd();
}

//--------------------------------------------------------------------------------------------------------------------

void BruteForce::test2(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->matrix = matrix->getMatrix();

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }

    List *heap = new List();
    List *helper = new List();
    d = INT_MAX;
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
            if (matrix[v][u] != -1 && !alreadyVisited[u]) // Szukamy nieodwiedzonego jeszcze sąsiada
            {
                dh += matrix[v][u];        // Dodajemy wagę krawędzi v-u do sumy
                tsp2(start, u, dh, verticesHeap,
                     helperHeap);                 // Rekurencyjnie wywołujemy szukanie cyklu Hamiltona
                dh -= matrix[v][u];        // Usuwamy wagę krawędzi z sumy
            }
        alreadyVisited[v] = false;         // Zwalniamy bieżący wierzchołek
    } else if (matrix[start][v] != -1)         // Jeśli znaleziona ścieżka jest cyklem Hamiltona
    {
        dh += matrix[v][start];           // to sprawdzamy, czy ma najmniejszą sumę wag
        if (dh < d)                   // Jeśli tak,
        {
            d = dh;                      // To zapamiętujemy tę sumę
            verticesHeap->copyOf(helperHeap); // oraz kopiujemy stos Sh do S
        }
        dh -= matrix[v][start];           // Usuwamy wagę krawędzi v-v0 z sumy
    }
    helperHeap->popEnd();                         // Usuwamy bieżący wierzchołek ze ścieżki
}
