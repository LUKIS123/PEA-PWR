#include "BranchAndBound.h"

#define INF INT_MAX


BranchAndBound::BranchAndBound() {

}

BranchAndBound::~BranchAndBound() {

}

void BranchAndBound::displayLatestResults() {
    std::cout << "\n";
    for (auto &i: withBest) {
        std::cout << i.first + 1 << " -> "
                  << i.second + 1 << std::endl;
    }
}

void BranchAndBound::mainFun(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->inputMatrix = matrix->getMatrix();
    this->distanceBest = INF;

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

    auto *start = new BranchAndBoundNode(rootMatrix, matrixSize);

    std::vector<std::pair<int, int>> w;
    std::vector<std::pair<int, int>> wo;
    start->with = w;
    start->without = wo;

    start->location = 0;
    start->upperBound = reduction;

    solve(start);
    std::cout << "\nCost is " << distanceBest << std::endl;
}

void BranchAndBound::solve(BranchAndBoundNode *root) {
    std::stack<BranchAndBoundNode *> lifoQueue;
    lifoQueue.push(root);

    while (!lifoQueue.empty()) {
        BranchAndBoundNode *node = lifoQueue.top();
        lifoQueue.pop();

        if (node->upperBound > distanceBest) {
            delete node;
            continue;
        }

        // Przeszukiwanie w glab dopoki nie pozostanie macierz 2x2
        //if (node->location == matrixSize - 2) {
        if (node->with.size() == matrixSize - 2) {
            auto rem = addRemainingEdgesOfOpportunityMatrix(node->data, node->size, node->with);
            for (const auto &item: rem) {
                node->upperBound += item.second;
                node->with.push_back(item.first);
            }
            if (node->upperBound < distanceBest) {
                distanceBest = node->upperBound;
                withBest = std::vector<std::pair<int, int >>(node->with);
                bestNodeFound = new BranchAndBoundNode(*node);
            }

            BranchAndBoundNode::sort_stack_cmp(lifoQueue);
            delete node;
            continue;
        }

        auto values = chooseWorstCase(node->data, node->size);
        int row = values.second.first;
        int column = values.second.second;

        std::vector<pair<int, int>> tmp = node->with;
        tmp.emplace_back(row, column);

        auto br = splitBranches(node->data, node->size, row, column);
        int cl = updateMatrixLeft(br.first, node->size, tmp);
        int cr = updateMatrixRight(br.second, node->size, row, column);

        auto *left = new BranchAndBoundNode(br.first, node->size);
        left->location = node->location + 1;
        left->upperBound = node->upperBound + cl;
        left->with.insert(std::end(left->with), std::begin(node->with), std::end(node->with));
        left->with.emplace_back(row, column);
        left->without.insert(std::end(left->without), std::begin(node->without), std::end(node->without));

        auto *right = new BranchAndBoundNode(br.second, node->size);
        right->location = node->location + 1;
        right->upperBound = node->upperBound + cr;
        right->without.insert(std::end(right->without), std::begin(node->without), std::end(node->without));
        right->without.emplace_back(row, column);
        right->with.insert(std::end(right->with), std::begin(node->with), std::end(node->with));

        if (cl < cr) {
            lifoQueue.push(right);
            lifoQueue.push(left);
        } else {
            lifoQueue.push(left);
            lifoQueue.push(right);
        }

        delete node;
    }

}

std::vector<std::pair<std::pair<int, int>, int>>
BranchAndBound::addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::vector<pair<int, int>> &with) {
    std::vector<std::pair<std::pair<int, int>, int >> result;
    auto remaining = getAllRemainingEdges(matrix, size);

    // TODO nie dziala dobieranie  - wczesniej out byl 1 kratka z macierzy 2x2 ale nie sprawdza sie dla generowanych
    // byc moze metoda to path , pozniej jeden luk z brakujacych samych
    // a pozniej jeszcze +1 zawiracacy taki ktory nie byl wybrany

    std::vector<int> firstVerticesRemaining;
    std::vector<int> secondVerticesRemaining;
    for (int i = 0; i < size; i++) {
        auto itFirst = std::find_if(with.begin(), with.end(),
                                    [&i](const pair<int, int> &p) { return p.first == i; });
        auto itSecond = std::find_if(with.begin(), with.end(),
                                     [&i](const pair<int, int> &p) { return p.second == i; });
        if (itFirst == with.end()) {
            firstVerticesRemaining.push_back(i);
        }
        if (itSecond == with.end()) {
            secondVerticesRemaining.push_back(i);
        }
    }
    std::vector<int> firstVerticesFound;
    std::vector<int> secondVerticesFound;
    std::pair<std::pair<int, int>, std::pair<int, int>> match;
    for (const auto &item1: remaining) {
        for (const auto &item2: remaining) {
            if (item1 == item2) continue;
            firstVerticesFound.push_back(item1.first);
            firstVerticesFound.push_back(item2.first);
            secondVerticesFound.push_back(item1.second);
            secondVerticesFound.push_back(item2.second);

            std::sort(firstVerticesFound.begin(), firstVerticesFound.end());
            std::sort(firstVerticesRemaining.begin(), firstVerticesRemaining.end());
            std::sort(secondVerticesFound.begin(), secondVerticesFound.end());
            std::sort(secondVerticesRemaining.begin(), secondVerticesRemaining.end());
            if (firstVerticesFound == firstVerticesRemaining && secondVerticesFound == secondVerticesRemaining) {
                match = std::make_pair(item1, item2);
                break;
            } else {
                firstVerticesFound.clear();
                secondVerticesFound.clear();
            }
        }
    }

    std::cout << "\n=============\n";
    for (auto &i: remaining) {
        std::cout << i.first + 1 << " -> "
                  << i.second + 1 << std::endl;
    }
    std::cout << "\n======DODANE=======\n";
    std::cout << match.first.first << " " << match.first.second << "||" << match.second.first << " "
              << match.second.second;
    std::cout << "\n=============\n";

    result.push_back({{match.first.first, match.first.second}, matrix[match.first.first][match.first.second]});
    result.push_back({{match.second.first, match.second.second}, matrix[match.second.first][match.second.second]});
    return result;

//    std::cout << "\n=============\n";
//    for (auto &i: remaining) {
//        std::cout << i.first + 1 << " -> "
//                  << i.second + 1 << std::endl;
//    }
//    std::cout << "\n======DODANE=======\n";
//    std::cout << match.first.first << " " << match.first.second << "||" << match.second.first << " "
//              << match.second.second;
//    std::cout << "\n=============\n";
/////////////////////////////////////

//    std::cout << "\n=============\n";
//    for (auto &i: remaining) {
//        std::cout << i.first + 1 << " -> "
//                  << i.second + 1 << std::endl;
//    }
//    std::cout << "\n=============\n";
//
//    std::vector<int> vertexOccurrences;
//    for (const auto &item: with) {
//        vertexOccurrences.push_back(item.first);
//        vertexOccurrences.push_back(item.second);
//    }
//
//    std::vector<int> incomplete;
//    for (const auto &item: vertexOccurrences) {
//        if (std::count(vertexOccurrences.begin(), vertexOccurrences.end(), item) != 2) {
//            if (std::find(incomplete.begin(), incomplete.end(), item) == incomplete.end()) {
//                incomplete.push_back(item);
//            }
//        }
//    }
//    std::cout << "\n=============\n";
//    for (const auto &item: incomplete) {
//        std::cout << item + 1 << std::endl;
//
//    }
//    std::cout << "\n=============\n";
//
//    for (const auto &item: remaining) {
////        if ((item.first == incomplete.first && item.second == incomplete.second) ||
////            (item.first == incomplete.second && item.second == incomplete.first)) {
////            continue;
////        }
//        if ((std::find(incomplete.begin(), incomplete.end(), item.first) != incomplete.end()) &&
//            (std::find(incomplete.begin(), incomplete.end(), item.second) != incomplete.end())) {
//            continue;
//        }
//
//        result.push_back({{item.first, item.second}, matrix[item.first][item.second]});
//    }
//    return result;
}

std::vector<std::pair<int, int>> BranchAndBound::getAllRemainingEdges(int **matrix, int size) {
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
    return {branch1Matrix, branch2Matrix};
}

// najpierw idzie chooseWorstCase -> potem split i po splicie trzeba wywolywac ta funckje
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

std::pair<int, std::pair<int, int>>
BranchAndBound::chooseWorstCase(int **matrix, int size) {
    int x = 0, y = 0;
    int value = -1;
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
    return std::make_pair(value, std::make_pair(x, y));
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
