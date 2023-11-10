#include <queue>
#include "BranchAndBound.h"

#define INF INT_MAX


void BranchAndBound::displayLatestResults() {
    for (auto &i: withBest) {
        std::cout << "( " << i.first << " -> "
                  << i.second << " ), \n";
    }
    std::cout << std::endl;
    std::cout << "Path: ";
    for (const auto &item: pathBest) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Distance: " << distanceBest << std::endl;
}

void BranchAndBound::mainFun(Matrix *matrix, int matrixSize) {
    this->matrixSize = matrixSize;
    this->inputMatrix = matrix->getMatrix();
    this->distanceBest = INF;
    this->vertices = (matrixSize * (matrixSize - 1)) / 2;

    withBest.clear();
    pathBest.clear();
    pathCurrentNode.clear();

    int **rootMatrix = new int *[matrixSize];
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

    solveTSP(start);
}

void BranchAndBound::solveTSP(BranchAndBoundNode *root) {
    std::priority_queue<BranchAndBoundNode *, std::vector<BranchAndBoundNode *>, BranchAndBoundNode::comp> QUEUE;
    QUEUE.push(root);

    while (!QUEUE.empty()) {
        BranchAndBoundNode *node = QUEUE.top();
        QUEUE.pop();

        // czy to rozwiazanie nie jest dobrze -> najpierw dodane rozwiazanie 41, pozniej 39 gdzie wczesniej od razu dawal 39
        //|| node->location >= 2 * matrixSize
        if (node->upperBound >= distanceBest) {
            delete node;
            continue;
        }

        // Przeszukiwanie w glab dopoki nie pozostanie macierz 2x2
        if (node->with.size() == matrixSize - 2) {
            bool outIsSuccess = false;
            auto rem = addRemainingEdgesOfOpportunityMatrix(node->data, node->size, node->with, outIsSuccess);
            if (!outIsSuccess) {
                // todo
                std::cout << "\n============ Error ============ \n";
                delete node;
                continue;
            }

            for (const auto &item: rem) {
                node->upperBound += item.second;
                node->with.push_back(item.first);
            }
            if (node->upperBound < distanceBest) {
                distanceBest = node->upperBound;
                withBest = std::vector<std::pair<int, int >>(node->with);
                pathBest = pathCurrentNode;
            }

            std::cout << "Rozwiazanie: " << distanceBest << std::endl;

            delete node;
            continue;
        }

        bool outEdgeChosen = false;
        auto values = chooseBestCaseEdge(node->data, node->size, outEdgeChosen);
        if (!outEdgeChosen) {
            // todo
            std::cout << "\n============ FAILURE ============ " << QUEUE.size() << "\n";
            //
            delete node;
            continue;
        }
        int row = values.second.first;
        int column = values.second.second;

        std::vector<pair<int, int>>
                tmp = node->with;
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

        QUEUE.push(right);
        QUEUE.push(left);

        delete node;
    }

}

std::vector<std::pair<std::pair<int, int>, int>>
BranchAndBound::addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::vector<pair<int, int>> &with,
                                                     bool &outSuccess) {
    std::vector<std::pair<std::pair<int, int>, int >> result;
    auto remaining = getAllRemainingEdges(matrix, size);

    std::pair<std::pair<int, int>, std::pair<int, int>> match = {{0, 0},
                                                                 {0, 0}};
    bool success = false;
    for (const auto &item1: remaining) {
        if (success) break;
        for (const auto &item2: remaining) {
            if (item2 == item1) continue;
            bool p = tryMakePath(with, item1, item2);
            if (p) {
                match = std::make_pair(item1, item2);
                success = true;
                break;
            }
        }
    }
    outSuccess = success;
    result.push_back({{match.first.first, match.first.second}, matrix[match.first.first][match.first.second]});
    result.push_back({{match.second.first, match.second.second}, matrix[match.second.first][match.second.second]});
    return result;
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
                // Wykluczenie calego wiersza lewej macierzy
                branch1Matrix[i][j] = INF;
                if (j != column) {
                    branch2Matrix[i][j] = matrix[i][j];
                } else {
                    branch2Matrix[i][j] = INF;
                }
            }
            continue;
        }
        for (int j = 0; j < size; j++) {
            branch2Matrix[i][j] = matrix[i][j];
            // Wykluczenie calej kolumny lewej macierzy
            if (j == column) {
                branch1Matrix[i][j] = INF;
                continue;
            }
            if (i == column && j == row) {
                branch1Matrix[i][j] = INF;
                continue;
            }
            branch1Matrix[i][j] = matrix[i][j];
        }
    }
    return {branch1Matrix, branch2Matrix};
}

int BranchAndBound::updateMatrixLeft(int **matrix, int size, const std::vector<pair<int, int>> &with) {
    // Iterowanie po lukach sciezki, jesli luki tworza czesciowa sciezke -> INF
    for (auto &i: with) {
        matrix[i.second][i.first] = INF;
        /////
        for (auto &j: with) {
            if (i == j) continue;
            if (j.second == i.first) {
                matrix[i.second][j.first] = INF;
                //matrix[j.second][i.first] = INF;
            }
            /////////////
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
    int *minValues = new int[size];
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
    delete[]minValues;
    return sum;
}

int BranchAndBound::reduceColumns(int **matrix, int size) {
    int *minValues = new int[size];
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
    delete[]minValues;
    return sum;
}

std::pair<int, std::pair<int, int>>
BranchAndBound::chooseBestCaseEdge(int **matrix, int size, bool &outSuccess) {
    int x = 0, y = 0;
    int value = INT_MIN;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (matrix[i][j] == 0) {
                int minimum = getMinimumDefined(matrix, i, j, size);
                if (minimum > value) {
                    value = minimum;
                    x = i;
                    y = j;
                    outSuccess = true;
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

bool BranchAndBound::tryMakePath(const std::vector<pair<int, int>> &with, std::pair<int, int> firstPair,
                                 std::pair<int, int> secondPair) {
    std::vector<pair<int, int>> tmp = std::vector<pair<int, int>>(with);
    tmp.push_back(firstPair);
    tmp.push_back(secondPair);

    pair<int, int> firstElement = tmp.at(0);
    pair<int, int> currentElement = tmp.at(0);
    std::vector<int> path;
    path.push_back(currentElement.first);
    while (path.size() != matrixSize + 1) {
        auto it = std::find_if(tmp.begin(), tmp.end(),
                               [&currentElement](pair<int, int> element) {
                                   return element.first == currentElement.second;
                               });
        if (it != tmp.end()) {
            path.push_back(currentElement.second);
            currentElement = *it;
        } else {
            return false;
        }

        if (currentElement == firstElement && path.size() != matrixSize + 1) {
            return false;
        }
    }
    pathCurrentNode = std::vector<int>(path);

    vector<int> check;
    check.push_back(firstElement.first);
    for (int i = 0; i < matrixSize; i++) {
        check.push_back(i);
    }

    std::sort(check.begin(), check.end());
    std::sort(path.begin(), path.end());

    if (check == path) {
        return true;
    }
    return false;
}

bool BranchAndBound::checkForLoops(const std::vector<pair<int, int>> &with) {
    for (const auto &item: with) {
        auto it = std::find_if(with.begin(), with.end(),
                               [&item](pair<int, int> element) {
                                   return element.second == item.first;
                               });
        if (it != with.end()) {
            return true;
        }
    }
    return false;
}


//            //30///////////////////////////
////            std::cout << "  ";
////            for (int i = 0; i < matrixSize; i++) {
////                if (i > 10) std::cout << " ";
////                else std::cout << "  ";
////                std::cout << i;
////            }
////            std::cout << "\n\n";
////            for (int i = 0; i < matrixSize; i++) {
////                std::cout << i;
////                if (i < 10) std::cout << " ";
////                for (int j = 0; j < matrixSize; j++) {
////                    std::cout << "  ";
////                    std::cout << node->data[i][j];
////                }
////                std::cout << std::endl;
////            }

