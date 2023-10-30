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
    std::cout << "Distance: " << currentBestDistance << std::endl;
}

// Funkcja wywolujaca wyszukiwanie cyklu Hamiltona metoda Brute Force
void BruteForce::mainFun(Matrix *matrix, int matrixSize, bool runOptimized) {
    this->matrixSize = matrixSize;
    this->matrix = matrix->getMatrix();

    alreadyVisited = new bool[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        alreadyVisited[i] = false;
    }
    currentBestDistance = INT_MAX;
    int helperSum = 0;

    if (runOptimized) {
        TSP_OPT(0, 0, helperSum);
    } else {
        TSP(0, 0, helperSum);
    }
}

// Metoda wyszukiwania cyklu Hamiltona o najmniejszej wadze (Brute Force)
void BruteForce::TSP(int currentVertex, int startVertex, int &helperSum) {
    // Dodanie biezacego wierzcholka na stos (zapamietywanie sciezki)
    temporaryStack->pushEnd(currentVertex);

    // Jesli stos zawiera cykl Hamiltona i istnieje polaczenie z wierzcholkiem startowym
    if (temporaryStack->getSize() == matrixSize && matrix[startVertex][currentVertex] != -1) {
        // Dodanie wagi drogi do wierzcholka startowego i skopiowanie sciezki
        helperSum += matrix[currentVertex][startVertex];
        if (helperSum < currentBestDistance) {
            currentBestDistance = helperSum;
            stack->copyOf(temporaryStack);
            stack->pushEnd(startVertex);
        }
        // Usuwanie wagi i usuwanie ze sciezki powrotnej
        helperSum -= matrix[currentVertex][startVertex];
        temporaryStack->popEnd();
        return;
    }
    // Dodanie wierzcholka do odwiedzonych
    alreadyVisited[currentVertex] = true;
    // Badanie sasiadow
    for (int i = 0; i < matrixSize; i++) {
        // Jesli wierzcholek odwiedzony lub droga z biezacego wierzcholka nie istnieje -> continue
        if (alreadyVisited[i] || matrix[currentVertex][i] == -1) {
            continue;
        }
        // Dodanie wagi do sumy tymczasowej
        helperSum += matrix[currentVertex][i];
        // Wywolywanie rekurencyjne dla kolejnego badanego sasiada
        TSP(i, startVertex, helperSum);
        // Usuwanie wagi z sumy pomocniczej
        helperSum -= matrix[currentVertex][i];
    }
    // Zwalnianie biezacego wierzcholka i usuwanie ze stosu
    alreadyVisited[currentVertex] = false;
    temporaryStack->popEnd();
}

// Metoda wyszukiwania cyklu Hamiltona o najmniejszej wadze (Brute Force) optymalizowana
// Pomijane sa niektore permutacje wiec nie byly wykonywane testy
void BruteForce::TSP_OPT(int currentVertex, int startVertex, int &helperSum) {
    // Dodanie biezacego wierzcholka na stos (zapamietywanie sciezki)
    temporaryStack->pushEnd(currentVertex);

    // Jesli stos zawiera cykl Hamiltona i istnieje polaczenie z wierzcholkiem startowym
    if (temporaryStack->getSize() == matrixSize && matrix[startVertex][currentVertex] != -1) {

        // Dodanie wagi drogi do wierzcholka startowego i skopiowanie sciezki
        helperSum += matrix[currentVertex][startVertex];
        if (helperSum < currentBestDistance) {
            currentBestDistance = helperSum;
            stack->copyOf(temporaryStack);
            stack->pushEnd(startVertex);
        }

        // Usuwanie wagi i usuwanie ze sciezki powrotnej
        helperSum -= matrix[currentVertex][startVertex];
        temporaryStack->popEnd();
        return;
    }

    // Dodanie wierzcholka do odwiedzonych
    alreadyVisited[currentVertex] = true;
    // Badanie sasiadow
    for (int i = 0; i < matrixSize; i++) {
        // Jesli wierzcholek odwiedzony lub droga z biezacego wierzcholka nie istnieje - continue
        if (alreadyVisited[i] || matrix[currentVertex][i] == -1) {
            continue;
        }
        // Dodanie wagi do sumy pomocniczej
        helperSum += matrix[currentVertex][i];

        // Optymalizacja 1: Odcinanie sciezki, która już przekracza obecną minimalną odległość
        if (helperSum > currentBestDistance) {
            helperSum -= matrix[currentVertex][i];
            continue;
        }
        // Optymalizacja 2 :Sledzenie minimalnej odleglosci, ktora nalezy dodac, aby ukonczyc biezaca ściezke
        if ((helperSum + minDistanceToFinish(alreadyVisited)) > currentBestDistance) {
            helperSum -= matrix[currentVertex][i];
            continue;
        }

        // Wywolywanie rekurencyjne dla kolejnego badanego sasiada
        TSP_OPT(i, startVertex, helperSum);
        // Usuwanie wagi z sumy pomocniczej
        helperSum -= matrix[currentVertex][i];
    }
    // Zwalnianie biezacego wierzcholka
    alreadyVisited[currentVertex] = false;
    // Usuwanie biezacego wierzcholka ze stosu
    temporaryStack->popEnd();
}

// Metoda wyliczajaca minimalna sume wag dla danej sciezki
int BruteForce::minDistanceToFinish(bool *visited) {
    int distanceToFinish = 0;
    for (int i = 0; i < matrixSize; i++) {
        int dist = INT_MAX;
        // Pomijamy odwiedzone wierzcholki
        if (visited[i]) {
            continue;
        }
        // Szukamy najbardziej korzystne polaczenie dla danego wierzcholka
        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {
                continue;
            }
            // Dla kazdego wierzcholka wyszukujemy sasiada o najmniejszej wadze i zapamietujemy
            dist = std::min(dist, matrix[i][j]);
        }

        // Dodajemy do sumy koncowej
        distanceToFinish += dist;
    }
    return distanceToFinish;
}
