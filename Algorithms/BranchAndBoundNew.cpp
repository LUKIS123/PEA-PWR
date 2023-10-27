#include "BranchAndBoundNew.h"


int BranchAndBoundNew::mainFun(Matrix *matrix, int matrixSize) {
    this->inputMatrix = matrix->getMatrix();
    this->matrixSize = matrixSize;
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
    std::cout << "\nCost is " << solve(reducedMatrix) << std::endl;

    return 0;
}

int BranchAndBoundNew::solve(int **adjacencyMatrix) {
    std::priority_queue<Node *, std::vector<Node *>, Node::comp> pq;
    std::vector<std::pair<int, int>> v;
    Node *root = Node::newNode(adjacencyMatrix, matrixSize, v, 0, -1, 0);
    root->cost = cost_calculation(root->matrix_reduced, matrixSize);
    pq.push(root);
    while (!pq.empty()) {
        Node *min = pq.top();
        pq.pop();
        int i = min->vertex;
        if (min->level == matrixSize - 1) {
            min->path.emplace_back(i, 0);
            printPath(min->path);
            return min->cost;
        }

        for (int j = 0; j < matrixSize; j++) {
            if (min->matrix_reduced[i][j] != INF) {
                Node *child = Node::newNode(min->matrix_reduced, matrixSize, min->path,
                                            min->level + 1, i, j);

                child->cost = min->cost + min->matrix_reduced[i][j]
                              + cost_calculation(child->matrix_reduced, matrixSize);

                pq.push(child);
            }
        }

        delete min;
    }
    return INF;
}

void BranchAndBoundNew::reduce_row(int **matrix_reduced, int *row, int size) {
    std::fill_n(row, size, INF);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix_reduced[i][j] < row[i]) {
                row[i] = matrix_reduced[i][j];
            }
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix_reduced[i][j] != INF && row[i] != INF) {
                matrix_reduced[i][j] -= row[i];
            }
        }
    }
}

void BranchAndBoundNew::reduce_column(int **matrix_reduced, int *col, int size) {
    std::fill_n(col, size, INF);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix_reduced[i][j] < col[j]) {
                col[j] = matrix_reduced[i][j];
            }
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (matrix_reduced[i][j] != INF && col[j] != INF) {
                matrix_reduced[i][j] -= col[j];
            }
        }
    }
}

int BranchAndBoundNew::cost_calculation(int **matrix_reduced, int size) {
    int cost = 0;
    int row[size];
    reduce_row(matrix_reduced, row, size);
    int col[size];
    reduce_column(matrix_reduced, col, size);

    for (int i = 0; i < size; i++) {
        cost += (row[i] != INT_MAX) ? row[i] : 0,
                cost += (col[i] != INT_MAX) ? col[i] : 0;
    }

    return cost;
}

void BranchAndBoundNew::printPath(std::vector<std::pair<int, int>> const &list) {
    std::cout << "\n";
    for (int i = 0; i < list.size(); i++) {
        std::cout << list[i].first + 1 << " -> "
                  << list[i].second + 1 << std::endl;
    }
}
