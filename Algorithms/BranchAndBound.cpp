#include "BranchAndBound.h"

#define INF INT_MAX

BranchAndBound::BranchAndBound() {

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

    std::cout << reduction << "\n";

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


//    for (int i = 0; i < matrixSize; i++) {
//        for (int j = 0; j < matrixSize; j++) {
//
//            std::cout << reducedMatrix[i][j] << " ";
//        }
//        std::cout << "\n";
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
            if (i == row && j == column) {
                //todo naprawic nigdy sie nie wykonuje
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
