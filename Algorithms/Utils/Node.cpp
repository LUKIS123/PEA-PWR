#include "Node.h"


Node *
Node::newNode(int **matrix_parent, int size, const std::vector<std::pair<int, int>> &path, int level, int i, int j) {
    Node *node = new Node;
    node->path = path;
    if (level != 0)
        node->path.emplace_back(i, j);

    int **reducedMatrix = new int *[size];
    for (int row = 0; row < size; row++) {
        reducedMatrix[row] = new int[size];
    }
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            if (row == column) {
                reducedMatrix[row][column] = INF;
                continue;
            }
            reducedMatrix[row][column] = matrix_parent[row][column];
        }
    }

    node->matrix_reduced = reducedMatrix;
    for (int k = 0; level != 0 && k < size; k++) {
        node->matrix_reduced[i][k] = INF;
        node->matrix_reduced[k][j] = INF;
    }

    node->matrix_reduced[j][0] = INF;
    node->level = level;
    node->vertex = j;
    return node;
}
