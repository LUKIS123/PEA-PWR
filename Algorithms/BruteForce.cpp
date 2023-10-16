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

//--------------------------------------------------------------------------------------------------------------------

void BruteForce::mainFun(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->matrix = matrix;

//    List *heap = new List();
//    List *helper = new List();

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }

    distance = INT_MAX;
    int helperSum = 0;

    //TSP(0, 0, helperSum, heap, helper);
    TSP_POP(0, 0, helperSum);

//    if (heap->getSize() != 0) {
//        heap->displayFromFront();
//        std::cout << distance << std::endl;
//    } else {
//        std::cout << "Lipa" << std::endl;
//    }
//    std::cout << "test" << std::endl;
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
        if (currentVertice != startVertice) {
            helperHeap->popEnd();
            return;
        }
    }

    //k10
    alreadyVisited[currentVertice] = true;
    for (int u = 0; u < matrixSize; u++) {
        //k12
        if (alreadyVisited[u] || matrix->getMatrix()[currentVertice][u] == -1) {
            continue;
        }
        helperSum += matrix->getMatrix()[currentVertice][u];

        // optymalizacja : There is no added value in extending a path that is already above the current minimal distance
        if (helperSum > distance) {
            helperSum -= matrix->getMatrix()[currentVertice][u];
            continue;
        }
        // opt 2
//        if (helperSum + minDistanceToFinish(todoPoints) > distance) {
//            helperSum -= matrix->getMatrix()[currentVertice][u];
//            continue;
//        }
        // optymalizacja

        TSP(u, startVertice, helperSum, verticesHeap, helperHeap);
        helperSum -= matrix->getMatrix()[currentVertice][u];
    }
    //k16
    alreadyVisited[currentVertice] = false;
    // k17
    helperHeap->popEnd();
}

//--------------------------------------------------------------------------------------------------------------------

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

void BruteForce::TSP_POP(int currentVertex, int startVertex, int &helperSum) {

    temporaryStack->pushEnd(currentVertex);

    //k2
    if (temporaryStack->getSize() == matrixSize && matrix->getMatrix()[startVertex][currentVertex] != -1) {
        //k4
        helperSum += matrix->getMatrix()[currentVertex][startVertex];
        if (helperSum < distance) {
            distance = helperSum;
            stack->copyOf(temporaryStack);

        }
        //k8
        helperSum -= matrix->getMatrix()[currentVertex][startVertex];
        temporaryStack->popEnd();
        return;
    }

    // k3
    if (matrix->getMatrix()[currentVertex][startVertex] == -1) {
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
        if (alreadyVisited[u] || matrix->getMatrix()[currentVertex][u] == -1) {
            continue;
        }
        helperSum += matrix->getMatrix()[currentVertex][u];

        // optymalizacja : There is no added value in extending a path that is already above the current minimal distance
        if (helperSum > distance) {
            helperSum -= matrix->getMatrix()[currentVertex][u];
            continue;
        }
        // opt 2
//        if (helperSum + minDistanceToFinish(todoPoints) > distance) {
//            helperSum -= matrix->getMatrix()[currentVertex][u];
//            continue;
//        }
        // optymalizacja

        TSP(u, startVertex, helperSum, stack, temporaryStack);
        helperSum -= matrix->getMatrix()[currentVertex][u];
    }
    //k16
    alreadyVisited[currentVertex] = false;
    // k17
    temporaryStack->popEnd();
}


//void BruteForce::mainFun() {
//    List *heap = new List();
//    List *helper = new List();
//
//    alreadyVisited = new bool[matrixSize];
//    for (int i = 0; i < matrixSize; i++) {
//        alreadyVisited[i] = false;
//    }
//
//    distance = INT_MAX;
//    int helperSum = 0;
//    TSP(0, 0, helperSum, heap, helper);
//    if (heap->getSize() != 0) {
//        heap->displayFromFront();
//        std::cout << distance << std::endl;
//    } else {
//        std::cout << "Lipa" << std::endl;
//    }
//}
//
//void BruteForce::TSP(int currentVertice, int startVertice, int &helperSum, List *verticesHeap, List *helperHeap) {
//
//    helperHeap->pushEnd(currentVertice);
//
//    //k2
//    if (helperHeap->getSize() == matrixSize && matrix->getMatrix()[startVertice][currentVertice] != -1) {
//        //k4
//        helperSum += matrix->getMatrix()[currentVertice][startVertice];
//        if (helperSum < distance) {
//            distance = helperSum;
//            verticesHeap->copyOf(helperHeap);
//
//        }
//        //k8
//        helperSum -= matrix->getMatrix()[currentVertice][startVertice];
//        helperHeap->popEnd();
//        return;
//    }
//
//    // k3
//    if (matrix->getMatrix()[currentVertice][startVertice] == -1) {
//        //k17
//        if (currentVertice != startVertice) {
//            helperHeap->popEnd();
//            return;
//        }
//    }
//
//    //k10
//    alreadyVisited[currentVertice] = true;
//    for (int u = 0; u < matrixSize; u++) {
//        //k12
//        if (alreadyVisited[u] || matrix->getMatrix()[currentVertice][u] == -1) {
//            continue;
//        }
//        helperSum += matrix->getMatrix()[currentVertice][u];
//        TSP(u, startVertice, helperSum, verticesHeap, helperHeap);
//        helperSum -= matrix->getMatrix()[currentVertice][u];
//    }
//    //k16
//    alreadyVisited[currentVertice] = false;
//    // k17
//    helperHeap->popEnd();
//}