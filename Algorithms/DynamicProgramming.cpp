#include "DynamicProgramming.h"


DynamicProgramming::~DynamicProgramming() {
    clearData();
}

void DynamicProgramming::displayLatestResults() {
    std::cout << "Path: ";
    for (const auto &item: tour) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Distance: " << distanceBest << std::endl;
}

void DynamicProgramming::mainFun(Matrix *matrix, int matrixSize) {
    clearData();
    this->inputMatrix = matrix->getMatrix();
    this->matrixSize = matrixSize;

    /////////////////////////////////////
//    int **distanceMatrix = new int *[6];
//    for (int i = 0; i < 6; i++) {
//        distanceMatrix[i] = new int[6];
//    }
//    for (int i = 0; i < 6; i++) {
//        for (int j = 0; j < 6; j++) {
//            distanceMatrix[i][j] = 1000;
//        }
//    }
//    distanceMatrix[5][0] = 10;
//    distanceMatrix[1][5] = 12;
//    distanceMatrix[4][1] = 2;
//    distanceMatrix[2][4] = 4;
//    distanceMatrix[3][2] = 6;
//    distanceMatrix[0][3] = 8;
//
//    this->matrixSize = 6;
//    this->inputMatrix = distanceMatrix;
    ////////////////////////////////////////////

    memo = new int *[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        memo[i] = new int[1 << matrixSize];
    }
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < 1 << matrixSize; j++) {
            memo[i][j] = NULL;
        }
    }

    if (!tour.empty()) {
        tour.clear();
    }

    int startVertex = 0;
    solve(startVertex);

    // solveTSP(startVertex);
}

void DynamicProgramming::solveTSP(int startVertex) {
    int END_STATE = (1 << matrixSize) - 1;

    // Add all outgoing edges from the starting node to memo table.
    for (int end = 0; end < matrixSize; end++) {
        if (end == startVertex) continue;
        memo[end][(1 << startVertex) | (1 << end)] = inputMatrix[startVertex][end];
    }

    for (int r = 3; r <= matrixSize; r++) {
        for (const auto &subset: combinations(r, matrixSize)) {
            if (notIn(startVertex, subset)) continue;
            for (int next = 0; next < matrixSize; next++) {
                if (next == startVertex || notIn(next, subset)) continue;
                int subsetWithoutNext = subset ^ (1 << next);
                int minDist = INT_MAX;
                for (int end = 0; end < matrixSize; end++) {
                    if (end == startVertex || end == next || notIn(end, subset)) continue;
                    int newDistance = memo[end][subsetWithoutNext] + inputMatrix[end][next];
                    if (newDistance < minDist) {
                        minDist = newDistance;
                    }
                }
                memo[next][subset] = minDist;
            }
        }
    }

    // Connect tour back to starting node and minimize cost.
    for (int i = 0; i < matrixSize; i++) {
        if (i == startVertex) continue;
        int tourCost = memo[i][END_STATE] + inputMatrix[i][startVertex];
        if (tourCost < distanceBest) {
            distanceBest = tourCost;
        }
    }

    int lastIndex = startVertex;
    int state = END_STATE;
    tour.push_back(startVertex);

    // Reconstruct TSP path from memo table.
    for (int i = 1; i < matrixSize; i++) {

        int bestIndex = -1;
        int bestDist = INT_MAX;
        for (int j = 0; j < matrixSize; j++) {
            if (j == startVertex || notIn(j, state)) continue;
            int newDist = memo[j][state] + inputMatrix[j][lastIndex];
            if (newDist < bestDist) {
                bestIndex = j;
                bestDist = newDist;
            }
        }

        tour.push_back(bestIndex);
        state = state ^ (1 << bestIndex);
        lastIndex = bestIndex;
    }

    tour.push_back(startVertex);
    std::reverse(tour.begin(), tour.end());
}

bool DynamicProgramming::notIn(int element, int subset) {
    return ((1 << element) & subset) == 0;
}

std::vector<int> DynamicProgramming::combinations(int r, int n) {
    std::vector<int> subsets;
    combinations(0, 0, r, n, subsets);
    return subsets;
}

void DynamicProgramming::combinations(int set, int at, int r, int n, std::vector<int> subsets) {
    // Return early if there are more elements left to select than what is available.
    int elementsLeftToPick = n - at;
    if (elementsLeftToPick < r) return;

    // We selected 'r' elements, so we found a valid subset!
    if (r == 0) {
        subsets.push_back(set);
    } else {
        for (int i = at; i < n; i++) {
            // Try including this element
            set ^= (1 << i);

            combinations(set, i + 1, r - 1, n, subsets);

            // Backtrack and try the instance where we did not include this element
            set ^= (1 << i);
        }
    }
}

///////////////////////////////////////////////////

void DynamicProgramming::solve(int start) {
    START_NODE = start;
    int state = 1 << START_NODE;
    FINISHED_STATE = (1 << matrixSize) - 1;

    prev = new int *[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        prev[i] = new int[1 << matrixSize];
    }
    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < 1 << matrixSize; j++) {
            prev[i][j] = NULL;
        }
    }

    distanceBest = TSP(START_NODE, state);

    // Regenerate path
    int index = START_NODE;
    while (true) {
        tour.push_back(index);
        int nextIndex = prev[index][state];
        if (nextIndex == NULL) break;
        int nextState = state | (1 << nextIndex);
        state = nextState;
        index = nextIndex;
    }

    tour.push_back(START_NODE);
}

int DynamicProgramming::TSP(int i, int state) {
    // Done this tour. Return cost of going back to start node.
    if (state == FINISHED_STATE) return inputMatrix[i][START_NODE];

    // Return cached answer if already computed.
    if (memo[i][state] != NULL) return memo[i][state];

    int minCost = INT_MAX;
    int index = -1;
    for (int next = 0; next < matrixSize; next++) {

        // Skip if the next node has already been visited.
        if ((state & (1 << next)) != 0) continue;

        int nextState = state | (1 << next);
        int newCost = inputMatrix[i][next] + TSP(next, nextState);
        if (newCost < minCost) {
            index = next;
            minCost = newCost;
        }
    }

    prev[i][state] = index;
    return memo[i][state] = minCost;
}

/////////////////////////////////////////////////

// Zwalnianie pamieci
void DynamicProgramming::clearData() {
    if (memo != nullptr) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] memo[i];
        }
        delete[] memo;
        memo = nullptr;
    }
    if (prev != nullptr) {
        for (int i = 0; i < matrixSize; i++) {
            delete[] prev[i];
        }
        delete[] prev;
        prev = nullptr;
    }
}
