#include "DynamicProgramming.h"


DynamicProgramming::~DynamicProgramming() {
    clearData();
}

void DynamicProgramming::displayLatestResults() {
    std::cout << "Path: ";
    for (const auto &item: path) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Distance: " << distanceBest << std::endl;
}

void DynamicProgramming::mainFun(Matrix *matrix, int matrixSize) {
    clearData();
    if (!path.empty()) {
        path.clear();
    }
    this->inputMatrix = matrix->getMatrix();
    this->matrixSize = matrixSize;

    cache = new int *[matrixSize];
    previous = new int *[matrixSize];

    for (int i = 0; i < matrixSize; i++) {
        cache[i] = new int[1 << matrixSize];
        previous[i] = new int[1 << matrixSize];
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < (1 << matrixSize); j++) {
            cache[i][j] = INT_MIN;
            previous[i][j] = INT_MIN;
        }
    }

    int startVertex = 0;
    solve(startVertex);
}

void DynamicProgramming::solve(int start) {
    START_NODE = start;
    FINISHED_STATE = (1 << matrixSize) - 1;
    int state = 1 << START_NODE;

    distanceBest = TSP(START_NODE, state);

//    std::cout << "  ";
//    for (int i = 0; i < matrixSize; i++) {
//        if (i > 10) std::cout << " ";
//        else std::cout << "  ";
//        std::cout << i;
//    }
//    std::cout << "\n\n";
//    for (int i = 0; i < matrixSize; i++) {
//        std::cout << i;
//        if (i < 10) std::cout << " ";
//        for (int j = 0; j < 1 << matrixSize; j++) {
//            std::cout << "  ";
//            if (previous[i][j] != INT_MIN) {
//                std::cout << previous[i][j];
//            } else {
//                std::cout << "X";
//            }
//
//        }
//        std::cout << std::endl;
//    }
//    std::cout << "  ";
//    for (int i = 0; i < matrixSize; i++) {
//        if (i > 10) std::cout << " ";
//        else std::cout << "  ";
//        std::cout << i;
//    }
//    std::cout << "\n\n";
//    for (int i = 0; i < matrixSize; i++) {
//        std::cout << i;
//        if (i < 10) std::cout << " ";
//        for (int j = 0; j < 1 << matrixSize; j++) {
//            std::cout << "  ";
//            if (cache[i][j] != INT_MIN) {
//                std::cout << cache[i][j];
//            } else {
//                std::cout << "X";
//            }
//
//        }
//        std::cout << std::endl;
//    }

    // Odtwarzanie wyznaczonej sciezki zaczynajac od wierzcholka startowego
    int index = START_NODE;
    while (true) {
        // Dodanie aktualnego wierzcholka do sciezki
        path.push_back(index);
        // Pobieranie nastepnego wierzcholka z pamieci
        int nextIndex = previous[index][state];
        // Jesli trafimy na koniec sciezki -> break
        if (nextIndex == INT_MIN) {
            break;
        }
        // Oznaczenie nastepnego wierzcholka w masce na 1
        int nextState = state | (1 << nextIndex);
        state = nextState;
        index = nextIndex;
    }

    // Dodanie wierzcholka startowego na koniec sciezki
    path.push_back(START_NODE);
}

int DynamicProgramming::TSP(int currentVertex, int currentState) {
    // Jesli wszystkie wierzcholki w galezi odwiedzone (KONIEC REKURENCJI), zwracamy koszt do wierzcholka startowego
    if (currentState == FINISHED_STATE) {
        return inputMatrix[currentVertex][START_NODE];
    }

    // Optymalizacja - Zwracanie kosztu jesli napotkano wczesniej taka sama kombinacje
    if (cache[currentVertex][currentState] != INT_MIN) {
        return cache[currentVertex][currentState];
    }

    int minCost = INT_MAX;
    int index = -1;
    // Przechodzimy po wszystkich wierzcholkach
    for (int V = 0; V < matrixSize; V++) {

        // Pozycja wierzcholka w masce
        int stateOfVertex = 1 << V;

        // Jesli wierzcholek zostal juz odwiedzony -> continue
        if ((currentState & stateOfVertex) != 0) {
            continue;
        }
        // Oznaczenie w masce wierzcholka V jako odwiedzony
        int nextState = currentState | stateOfVertex;
        // Rekurencyjne wyznaczanie kosztu sciezki ze zbioru pomniejszonego o wierzcholek V
        int S_NEXT = TSP(V, nextState);
        // (REKURENCYJNIE) Koszt = waga(CurrentVertex, V) + waga(V, S - {V})
        int newCost = inputMatrix[currentVertex][V] + S_NEXT;
        // Jesli koszt dla danego V jest najmniejszy
        if (newCost < minCost) {
            index = V;
            minCost = newCost;
        }
    }
    // Zapamietujemy dla ktorego kolejnego wierzcholka (jego indeks) koszt jest najmniejszy
    previous[currentVertex][currentState] = index;

    // Zapamietujemy koszt dla tego kolejnego wierzcholka
    // optymalizacja -zapisanie dla wierzcholka i danej sciezki (maski) jestgo kosztu
    cache[currentVertex][currentState] = minCost;

    // Zwracamy ten koszt dla wczesniejszego stanu sciezki
    return minCost;
}

// Zwalnianie pamieci
void DynamicProgramming::clearData() {
    if (cache != nullptr) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] cache[i];
        }
        delete[] cache;
        cache = nullptr;
    }
    if (previous != nullptr) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] previous[i];
        }
        delete[] previous;
        previous = nullptr;
    }
}

/*
 * czego użyłem jako listy odwiedzonych wierzcholkow i jak to dziala
 * jak sie zapisuje ta najlepsza sciezke
 * dla ilu max wierzcholkow mozna ten algorytm zrobic
 * ogół działania
 */