#include "BranchAndBound.h"

#define INF INT_MAX

BranchAndBound::BranchAndBound() {
    nodeList = new std::vector<BranchAndBoundNode *>;
}

BranchAndBound::~BranchAndBound() {

}

void BranchAndBound::displayLatestResults() {

}

void BranchAndBound::mainFun(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->inputMatrix = matrix->getMatrix();

    reducedMatrix = new int *[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        reducedMatrix[i] = new int[matrixSize];
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {
                reducedMatrix[i][j] = INF;
                continue;
            }
            reducedMatrix[i][j] = inputMatrix[i][j];
        }
    }

    int r = reduceRows(reducedMatrix, matrixSize);
    int c = reduceColumns(reducedMatrix, matrixSize);
    int reduction = c + r;

    // todo vector prawy node lewy i prawy
    BranchAndBoundNode *first = new BranchAndBoundNode(reducedMatrix, matrixSize);
    first->location = 0;
    first->upperBound = reduction;
    currentNode = first;
    nodeList->push_back(first);


    //TODO
    branchOut();
    return;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto values = chooseWorstCase(reducedMatrix, matrixSize);
//    std::cout << "waga " << values.first << " wiersz " << values.second.first << " kolumna " << values.second.second
//              << "\n";
    int highestWeight = values.first;
    int row = values.second.first;
    int column = values.second.second;


//    podział zbioru rozwiązań
//    Wybrany łuk to (6,3).
//            Zbiór rozwiązań dzielony jest na dwa podzbiory:
//    - lewe poddrzewo – rozwiązania z łukiem (6,3)
//    - prawe poddrzewo – rozwiązania bez łuku (6,3)

    auto branches = splitBranches(reducedMatrix, matrixSize, row, column);

    std::cout << "\n";
    std::cout << "\n";
    for (int i = 0; i < matrixSize - 1; i++) {
        for (int j = 0; j < matrixSize - 1; j++) {

            std::cout << branches.first[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {

            std::cout << branches.second[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    std::cout << "\n";

    int t1 = reduceRows(branches.second, matrixSize);
    int t2 = reduceColumns(branches.second, matrixSize);
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {

            std::cout << branches.second[i][j] << " ";
        }
        std::cout << "\n";
    }
    int lowerBound2 = reduction + t1 + t2;
    std::cout << "\n" << "t=" << lowerBound2;


    int t11 = reduceRows(branches.first, matrixSize - 1);
    int t12 = reduceColumns(branches.first, matrixSize - 1);
    int lowerBound1 = reduction + t11 + t12;
    std::cout << "\n" << "t=" << lowerBound1 << std::endl;

    // todo TERAZ WYBIERAMY BRANCHA O MNIEJSZYM LB CZYLI FIRST! o loweBound1
    // powrot do korkow 2 i3

    auto next = chooseWorstCase(branches.first, matrixSize - 1);
    std::cout << "waga " << next.first << " wiersz " << next.second.first << " kolumna " << next.second.second
              << "\n";

    // todo dodac parametr pamietajacy numery wierzcholkow? dla macierzy.. moze jakas tablica i kazdy indeks ma nr wierzcholka
//    Krok 6 – uzupełnianie trasy komiwojaŜera
//    W macierzy 2x2 dobieramy tak pozostałe dwa łuki, aby utworzyć kompletną trasę
//    komiwojaŜera (cykl Hamiltona).

    // todo jesli macierz jest 2x2
    // oblicz droge sposrod lukow, jesli LB jest wiekszy niz ktoras z odnog, to powrot

    // todo dolozyc pamiec z jakimi wierzchłkami droga♦


//    for (int i = 0; i < matrixSize; i++) {
//        for (int j = 0; j < matrixSize; j++) {
//
//            std::cout << reducedMatrix[i][j] << " ";
//        }
//        std::cout << "\n";
//    }
}

void BranchAndBound::branchOut() {
    BranchAndBoundNode *lowest = getLowestBoundNode();
//    while (currentNode->upperBound == lowest->upperBound && currentNode->data != reducedMatrix) {
    while (currentNode->size != 2) {
        auto values = chooseWorstCase(currentNode->data, currentNode->size);
        int highestWeight = values.first;
        int row = values.second.first;
        int column = values.second.second;

        // todo: zmienic -> nie bedziemy zmniejszac rozmiarow macierzy ++ widac caly czas indeksy oryginalne + zapisywac łuki w nodesie
        // TODO WAZNE

        auto branches = splitBranches(currentNode->data, currentNode->size, row, column);

        int r11 = reduceRows(branches.first, currentNode->size - 1);
        int r12 = reduceColumns(branches.first, currentNode->size - 1);

        int r21 = reduceRows(branches.second, currentNode->size);
        int r22 = reduceColumns(branches.second, currentNode->size);

        int lowerBound1 = currentNode->upperBound + r11 + r12;
        int lowerBound2 = currentNode->upperBound + r21 + r22;

        BranchAndBoundNode *left = new BranchAndBoundNode(branches.first, currentNode->size - 1);
        left->location = currentNode->location + 1;
        left->upperBound = lowerBound1;
        nodeList->push_back(left);

        BranchAndBoundNode *right = new BranchAndBoundNode(branches.second, currentNode->size);
        right->location = currentNode->location + 1;
        right->upperBound = lowerBound2;
        nodeList->push_back(right);


        std::cout << "========PRAWY=======\n";
        for (int i = 0; i < right->size; i++) {
            for (int j = 0; j < right->size; j++) {

                std::cout << right->data[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "t=" << right->upperBound << "\n\n=========LEWY==========\n";
        for (int i = 0; i < left->size; i++) {
            for (int j = 0; j < left->size; j++) {

                std::cout << left->data[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "t=" << left->upperBound << "\n\n";


        if (lowerBound1 < lowerBound2) {
            currentNode = left;
        } else {
            currentNode = right;
        }


    }
//    }

}

BranchAndBoundNode *BranchAndBound::getLowestBoundNode() {
    BranchAndBoundNode *found = nodeList->at(0);
    for (auto &i: *nodeList) {
        if (found->upperBound > i->upperBound) {
            found = i;
        }
    }

    return found;
//    for (auto i = nodeList->begin(); i != nodeList->end(); ++i) {
//        if (found->upperBound > (*i)->upperBound) {
//            found = *i;
//        }
//    }
}


int BranchAndBound::reduceRows(int **matrix, int size) {
    int minValues[size];
    int sum = 0;
    std::fill_n(minValues, size, INT_MAX);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                continue;
            }
            if (minValues[i] > matrix[i][j]) {
                minValues[i] = matrix[i][j];
            }
        }
        sum += minValues[i];
        for (int j = 0; j < size; j++) {
            if (i == j || matrix[i][j] == INF || matrix[i][j] == 0) {
                continue;
            }
            matrix[i][j] -= minValues[i];
        }
    }
    return sum;
}

int BranchAndBound::reduceColumns(int **matrix, int size) {
    int minValues[size];
    int sum = 0;
    std::fill_n(minValues, size, INT_MAX);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) {
                continue;
            }
            if (minValues[i] > matrix[j][i]) {
                minValues[i] = matrix[j][i];
            }
        }
        sum += minValues[i];
        for (int j = 0; j < size; j++) {
            if (i == j || matrix[j][i] == INF || matrix[j][i] == 0) {
                continue;
            }
            matrix[j][i] -= minValues[i];
        }
    }
    return sum;
}

std::pair<int, std::pair<int, int>> BranchAndBound::chooseWorstCase(int **matrix, int size) {
    int x, y;
    int value = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (matrix[i][j] == 0) {
                int minimum = getMinimumDefined(matrix, i, j, size);
                if (minimum > value) {
                    value = minimum;
                    x = i;
                    y = j;
                }
            }
        }
    }
    return {value, {x, y}};
}

int BranchAndBound::getMinimumDefined(int **matrix, int row, int column, int size) {
    int valueRow = INT_MAX;
    int valueColumn = INT_MAX;
    for (int i = 0; i < size; i++) {
        if (i != column && matrix[row][i] != INF && matrix[row][i] < valueRow) {
            valueRow = matrix[row][i];
        }
        if (i != row && matrix[i][column] != INF && matrix[i][column] < valueColumn) {
            valueColumn = matrix[i][column];
        }
    }
    return valueRow + valueColumn;
}

std::pair<int **, int **> BranchAndBound::splitBranches(int **matrix, int size, int row, int column) {
    int **branch1Matrix = new int *[size - 1];
    int **branch2Matrix = new int *[size];
    for (int i = 0; i < size; i++) {
        if (i < size - 1) {
            branch1Matrix[i] = new int[size - 1];
        }
        branch2Matrix[i] = new int[size];
    }

    int rowPointer = 0;
    for (int i = 0; i < size; i++) {
        if (i == row) {
            for (int j = 0; j < size; j++) {
                branch2Matrix[i][j] = matrix[i][j];
            }
            continue;
        }
        int columnPointer = 0;
        for (int j = 0; j < size; j++) {
            branch2Matrix[i][j] = matrix[i][j];
            if (j == column) {
                continue;
            }
            if (i == column && j == row) {
                branch1Matrix[rowPointer][columnPointer] = INF;
            } else {
                branch1Matrix[rowPointer][columnPointer] = matrix[i][j];
            }
            columnPointer++;
        }
        rowPointer++;
    }
    branch2Matrix[row][column] = INF;

    return {branch1Matrix, branch2Matrix};
}
