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

    rootMatrix = new int *[matrixSize];
    for (int i = 0; i < matrixSize; i++) {
        rootMatrix[i] = new int[matrixSize];
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            if (i == j) {
                rootMatrix[i][j] = INF;
                continue;
            }
            rootMatrix[i][j] = inputMatrix[i][j];
        }
    }

    int r = reduceRows(rootMatrix, matrixSize);
    int c = reduceColumns(rootMatrix, matrixSize);
    int reduction = c + r;

    BranchAndBoundNode *start = new BranchAndBoundNode(rootMatrix, matrixSize);

    std::vector<std::pair<int, int>> edges;
    start->path = edges;

    start->location = 0;
    start->upperBound = reduction;
    start->remainingVertices = matrixSize;
    start->branchedOut = true;


    solve(start);
    return;


    currentNode = start;
    nodeList->push_back(start);

    branchOut();
}

void BranchAndBound::solve(BranchAndBoundNode *root) {
    std::stack<BranchAndBoundNode *> lifoQueue;
    lifoQueue.push(root);
    while (!lifoQueue.empty()) {
        BranchAndBoundNode *node = lifoQueue.top();
        lifoQueue.pop();
        

    }


}

void BranchAndBound::branchOut() {
    while (getLowestBoundNode()->upperBound < currentNode->upperBound && currentNode->branchedOut &&
           currentNode->data != rootMatrix) {

        while (currentNode->remainingVertices != 2) {
            auto values = chooseWorstCase(currentNode->data, currentNode->size);
            int highestWeight = values.first;
            int row = values.second.first;
            int column = values.second.second;

            // dodawanie do sciezki nastepnego splita
            currentNode->path.emplace_back(values.second.first, values.second.second);
            std::vector<pair<int, int>> tmp(currentNode->with);
            tmp.emplace_back(row, column);

            auto br = splitBranches(currentNode->data, currentNode->size, row, column);

            int cl = updateMatrixLeft(br.first, matrixSize, tmp);
            int cr = updateMatrixRight(br.second, matrixSize, row, column);

            BranchAndBoundNode *left = new BranchAndBoundNode(br.first, currentNode->size);
            left->location = currentNode->location + 1;
            left->upperBound = currentNode->upperBound + cl;
            left->parent = currentNode;
            left->path.insert(std::end(left->path), std::begin(currentNode->path), std::end(currentNode->path));
            left->with.insert(std::end(left->with), std::begin(currentNode->with), std::end(currentNode->with));
            left->with.emplace_back(row, column);
            left->remainingVertices = currentNode->remainingVertices - 1;
            nodeList->push_back(left);

            BranchAndBoundNode *right = new BranchAndBoundNode(br.second, currentNode->size);
            right->location = currentNode->location + 1;
            right->upperBound = currentNode->upperBound + cr;
            right->parent = currentNode;
            right->path.insert(std::end(right->path), std::begin(currentNode->path), std::end(currentNode->path));
            right->without.insert(std::end(right->with), std::begin(currentNode->without),
                                  std::end(currentNode->without));
            right->without.emplace_back(row, column);
            right->remainingVertices = currentNode->remainingVertices - 1;
            nodeList->push_back(right);

            std::cout << "========LEWY=======\n";
            for (int i = 0; i < left->size; i++) {
                for (int j = 0; j < left->size; j++) {

                    std::cout << left->data[i][j] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "t=" << left->upperBound << "\n\n";
            std::cout << "========PRAWY=======\n";
            for (int i = 0; i < right->size; i++) {
                for (int j = 0; j < right->size; j++) {

                    std::cout << right->data[i][j] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "t=" << right->upperBound << "\n\n";


            if (cl < cr) {
                left->branchedOut = true;
                currentNode = left;
            } else {
                right->branchedOut = true;
                currentNode = right;
            }
        }


        auto rem = addRemainingEdges(currentNode->data);
        for (const auto &item: rem) {
            currentNode->remainingVertices -= 1;
            currentNode->upperBound += item.second;
            currentNode->with.push_back(item.first);
            currentNode->path.push_back(item.first);
        }
        distanceBest = currentNode->upperBound;
        withBest = std::vector<std::pair<int, int>>(currentNode->with);


        currentNode = getLowestBoundNode();
    }

    std::cout << "sciezka:";
    for (const auto &item: withBest) {
        std::cout << "(" << item.first << "," << item.second << "),";
    }

    std::cout << "\nt=" << distanceBest << "\n\n";

}

std::vector<std::pair<std::pair<int, int>, int>>
BranchAndBound::addRemainingEdges(int **matrix) {
    std::vector<std::pair<std::pair<int, int>, int>> result;
    auto remaining = getRemainingEdges(currentNode->data, currentNode->size);
    for (const auto &item: remaining) {
        int firstV = item.first;
        auto i = std::find_if(remaining.begin(), remaining.end(),
                              [&firstV](const std::pair<int, int> &element) { return element.second == firstV; });
        if (i != remaining.end()) {
            result.push_back({{item.first, item.second}, matrix[item.first][item.second]});
            result.push_back({{i->first, i->second}, matrix[i->first][i->second]});
        }
    }
    return result;
}

std::vector<std::pair<int, int>> BranchAndBound::getRemainingEdges(int **matrix, int size) {
    std::vector<std::pair<int, int>> rem;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (matrix[i][j] != INF) {
                rem.emplace_back(i, j);
            }
        }
    }
    return rem;
}

//std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>>
//BranchAndBound::checkConnections(const std::vector<std::pair<int, int>> &with) {
//    currentPathVertices = new std::vector<int>;
//    vector<std::pair<int, int>> complete;
//    vector<std::pair<int, int>> incomplete;
//    auto iterator = with.begin();
//
//    while ((complete.size() + incomplete.size()) != with.size()) {
//        pair<int, int> firstPair = *iterator;
//        int firstV = firstPair.first;
//
//        auto i = std::find_if(with.begin(), with.end(),
//                              [&firstV](const std::pair<int, int> &element) { return element.second == firstV; });
//        if (i != with.end()) {
//            complete.push_back(*iterator);
//        } else {
//            if (i->second != firstV) {
//                incomplete.push_back(*iterator);
//            }
//        }
//        currentPathVertices->push_back(iterator->first);
//        ++iterator;
//    }
//
//    return {complete, incomplete};
//}


BranchAndBoundNode *BranchAndBound::getLowestBoundNode() {
    int index = 0;
    int bound = nodeList->at(0)->upperBound;
    for (int i = 0; i < nodeList->size(); i++) {
        BranchAndBoundNode *node = nodeList->at(i);
        if (node->upperBound < bound && !node->branchedOut) {
            index = i;
        }
    }
    return nodeList->at(index);
}

std::pair<int **, int **> BranchAndBound::splitBranches(int **matrix, int size, int row, int column) {
    int **branch1Matrix = new int *[size];
    int **branch2Matrix = new int *[size];

    for (int i = 0; i < size; i++) {
        branch1Matrix[i] = new int[size];
        branch2Matrix[i] = new int[size];
    }

    for (int i = 0; i < size; i++) {
        if (i == row) {
            for (int j = 0; j < size; j++) {
                branch1Matrix[i][j] = INF;
                branch2Matrix[i][j] = matrix[i][j];
            }
            continue;
        }
        for (int j = 0; j < size; j++) {
            branch2Matrix[i][j] = matrix[i][j];
            if (j == column) {
                branch1Matrix[i][j] = INF;
                continue;
            }
            if (i == column && j == row) {
                branch1Matrix[i][j] = INF;
            } else {
                branch1Matrix[i][j] = matrix[i][j];
            }
        }
    }
    //branch2Matrix[row][column] = INF;
    return {branch1Matrix, branch2Matrix};
}

// TODO:
// najpier idzie chooseWorstCase -> potem split i po splicie trzeba wywolywac ta funckje
int BranchAndBound::updateMatrixLeft(int **matrix, int size, const std::vector<pair<int, int>> &with) {
    // przeiterowac po path, sprawdzic czy jakies sie nie powtarzaja krancami -> wtedy INF
    for (auto &i: with) {
        matrix[i.second][i.first] = INF;
        for (auto &j: with) {
            if (j.second == i.first) {
                matrix[i.second][j.first] = INF;
            }
        }
    }

    int rr = reduceRows(matrix, size);
    int rc = reduceColumns(matrix, size);
    int reduction = rr + rc;
    return reduction;
}

int BranchAndBound::updateMatrixRight(int **matrix, int size, int row, int column) {
    matrix[row][column] = INF;
    int rr = reduceRows(matrix, size);
    int rc = reduceColumns(matrix, size);
    int reduction = rr + rc;
    return reduction;
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
        if (minValues[i] != INT_MAX) {
            sum += minValues[i];
        }
        if (minValues[i] == 0 || minValues[i] == INT_MAX) {
            continue;
        }
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
            if (i == j || matrix[j][i] == INF) {
                continue;
            }
            if (minValues[i] > matrix[j][i]) {
                minValues[i] = matrix[j][i];
            }
        }
        if (minValues[i] != INT_MAX) {
            sum += minValues[i];
        }
        if (minValues[i] == 0 || minValues[i] == INT_MAX) {
            continue;
        }
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