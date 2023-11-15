#include "BranchAndBound.h"

#define INF INT_MAX


void BranchAndBound::displayLatestResults() {
    std::cout << "Edges:\n";
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
    withBest.clear();
    pathBest.clear();
    pathCurrentNode.clear();

    // Kopiowanie macierzy wejsciowej do pierwszego wezla naszego drzewa
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
    // Redukowanie macierzy pierwszego wezla
    int r = reduceRows(rootMatrix, matrixSize);
    int c = reduceColumns(rootMatrix, matrixSize);
    int reduction = c + r;

    // Tworzenie obiektu wezla, ustawienie LB jako koszt redukcji
    auto *start = new BranchAndBoundNode(rootMatrix, matrixSize);
    start->lowerBound = reduction;
    solveTSP(start);
}

// Glowna petla algorytmu B&B Little'a
void BranchAndBound::solveTSP(BranchAndBoundNode *root) {
    // Struktura wykorzystana do kolejki -> priority queue, sortujaca elementy po LB
    std::priority_queue<BranchAndBoundNode *, std::vector<BranchAndBoundNode *>, BranchAndBoundNode::comp> QUEUE;
    QUEUE.push(root);
    while (!QUEUE.empty()) {
        BranchAndBoundNode *node = QUEUE.top();
        QUEUE.pop();
        // Odrzucamy gorsze rozwiazania od najlepszego dotychczas
        if (node->lowerBound >= distanceBest) {
            delete node;
            continue;
        }
        // Przeszukiwanie dopoki nie pozostanie macierz 2x2
        if (node->with.size() == matrixSize - 2) {
            // Dodanie 2 ostatnich krawedzi z macierzy 2x2
            bool outHamiltonianCycle = false;
            auto rem = addRemainingEdgesOfOpportunityMatrix(node->data, node->size, node->with, outHamiltonianCycle);
            if (!outHamiltonianCycle) {
                delete node;
                continue;
            }
            // Zaktualizowanie kosztu LB oraz zapamietanie najlepszego rozwiazania
            for (const auto &item: rem) {
                node->lowerBound += item.second;
                node->with.push_back(item.first);
            }
            if (node->lowerBound < distanceBest) {
                distanceBest = node->lowerBound;
                withBest = std::list<std::pair<int, int >>(node->with);
                pathBest = pathCurrentNode;
            }
            delete node;
            continue;
        }
        // Wybierany jest luk, ktory powoduje najwiekszy wzrost dolnego ograniczenia dla rozwiazan niezawierajacych tego luku
        bool outEdgeFound = false;
        auto values = chooseBestCaseEdge(node->data, node->size, outEdgeFound);
        if (!outEdgeFound) {
            delete node;
            continue;
        }
        int row = values.second.first;
        int column = values.second.second;

        // Dodanie wybranego luku do listy krawedzi lewego poddrzewa
        std::list<pair<int, int>> leftSubtreeEdges = node->with;
        leftSubtreeEdges.emplace_back(row, column);

        // Podzial na lewe i prawe poddrzewo
        auto br = splitBranches(node->data, node->size, row, column);
        auto subToursReduced = buildSubToursFromEdges(std::make_pair(row, column), node->subTours);
        int cl = updateMatrixLeft(br.first, node->size, leftSubtreeEdges, subToursReduced);
        int cr = updateMatrixRight(br.second, node->size, row, column);

        // Tworzenie lewego wezla poddrzewa
        auto *left = new BranchAndBoundNode(br.first, node->size);
        left->lowerBound = node->lowerBound + cl;
        left->with.insert(std::end(left->with), std::begin(node->with), std::end(node->with));
        left->with.emplace_back(row, column);
        left->subTours = subToursReduced;

        // Tworzenie prawego wezla poddrzewa
        auto *right = new BranchAndBoundNode(br.second, node->size);
        right->lowerBound = node->lowerBound + cr;
        right->with.insert(std::end(right->with), std::begin(node->with), std::end(node->with));
        right->subTours = node->subTours;

        // Dodanie obiektow do kolejki
        QUEUE.push(right);
        QUEUE.push(left);
        delete node;
    }
}

// Funkcja tworzaca 2 macierze - lewego oraz prawego poddrzewa z macierzy wejsciowej
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

// Funkcja pomocnicza sluzaca do wpisania INF w celu zablokowania powstawania petli oraz redukcji macierzy lewego poddrzewa
int BranchAndBound::updateMatrixLeft(int **matrix, int size, const std::list<pair<int, int>> &with,
                                     const std::list<std::list<int>> &subTours) {
    // Iterowanie po lukach sciezki, jesli luki tworza czesciowa sciezke -> INF
    for (auto &i: with) {
        matrix[i.second][i.first] = INF;
        for (auto &j: with) {
            if (i == j) continue;
            if (j.second == i.first) {
                matrix[i.second][j.first] = INF;
            }
        }
    }
    for (const auto &item: subTours) {
        if (item.size() < 3) continue;
        matrix[item.back()][item.front()] = INF;
    }
    int rr = reduceRows(matrix, size);
    int rc = reduceColumns(matrix, size);
    int reduction = rr + rc;
    return reduction;
}

// Funkcja pomocnicza sluzaca do wpisania INF w celu zablokowania krawedzi "bez" - macierzy prawego poddrzewa
int BranchAndBound::updateMatrixRight(int **matrix, int size, int row, int column) {
    matrix[row][column] = INF;
    int rr = reduceRows(matrix, size);
    int rc = reduceColumns(matrix, size);
    int reduction = rr + rc;
    return reduction;
}

// Funckja sluzaca do redukcji wierzy macierzy
int BranchAndBound::reduceRows(int **matrix, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        int min = INT_MAX;
        for (int j = 0; j < size; j++) {
            if (i == j) {
                continue;
            }
            if (min > matrix[i][j]) {
                min = matrix[i][j];
            }
        }
        if (min != INT_MAX) {
            sum += min;
        }
        if (min == 0 || min == INT_MAX) {
            continue;
        }
        for (int j = 0; j < size; j++) {
            if (i == j || matrix[i][j] == INF || matrix[i][j] == 0) {
                continue;
            }
            matrix[i][j] -= min;
        }
    }
    return sum;
}

// Funckja sluzaca do redukcji kolumn macierzy
int BranchAndBound::reduceColumns(int **matrix, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        int min = INT_MAX;
        for (int j = 0; j < size; j++) {
            if (i == j || matrix[j][i] == INF) {
                continue;
            }
            if (min > matrix[j][i]) {
                min = matrix[j][i];
            }
        }
        if (min != INT_MAX) {
            sum += min;
        }
        if (min == 0 || min == INT_MAX) {
            continue;
        }
        for (int j = 0; j < size; j++) {
            if (i == j || matrix[j][i] == INF || matrix[j][i] == 0) {
                continue;
            }
            matrix[j][i] -= min;
        }
    }
    return sum;
}

// Funkcja sluzaca do wyboru luku, ktory powoduje najwiekszy wzrost dolnego ograniczenia dla rozwiazan niezawierajacych tego luku
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

// Funkcja pomocnicza sluzaca do wyboru kandydatow na wybor luku
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

// Funkcja dodajaca 2 pozostale krawedzie z macierzy 2x2
std::list<std::pair<std::pair<int, int>, int>>
BranchAndBound::addRemainingEdgesOfOpportunityMatrix(int **matrix, int size, const std::list<pair<int, int>> &with,
                                                     bool &outSuccess) {
    std::list<std::pair<std::pair<int, int>, int >> result;
    auto remaining = getAllRemainingEdges(matrix, size);
    std::pair<std::pair<int, int>, std::pair<int, int>> match = {{0, 0},
                                                                 {0, 0}};
    bool success = false;
    for (const auto &item1: remaining) {
        if (success) break;
        for (const auto &item2: remaining) {
            if (item2 == item1) continue;
            bool path = tryMakePath(with, item1, item2);
            if (path) {
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

// Funkcja pomocnicza zwracajaca pozostale krawedzie macirzy 2x2
std::list<std::pair<int, int>> BranchAndBound::getAllRemainingEdges(int **matrix, int size) {
    std::list<std::pair<int, int>> remaining;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == j) continue;
            if (matrix[i][j] != INF) {
                remaining.emplace_back(i, j);
            }
        }
    }
    return remaining;
}

// Funkcja pomocnicza do sprawdzenia czy 2 krawedzie wybrane w macierzy 2x2 tworza kompletna sciezke oraz zapisanie sciezki rozwiazania
bool BranchAndBound::tryMakePath(const std::list<pair<int, int>> &with, std::pair<int, int> firstPair,
                                 std::pair<int, int> secondPair) {
    std::list<pair<int, int>> tmp = std::list<pair<int, int>>(with);
    tmp.push_back(firstPair);
    tmp.push_back(secondPair);

    pair<int, int> firstElement = tmp.front();
    pair<int, int> currentElement = tmp.front();
    std::list<int> path;
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
    pathCurrentNode = std::list<int>(path);
    std::list<int> check;
    check.push_back(firstElement.first);
    for (int i = 0; i < matrixSize; i++) {
        check.push_back(i);
    }
    check.sort();
    path.sort();
    if (check == path) {
        return true;
    }
    return false;
}

// Funkcja sluzaca do budowania lancuchow podsciezek w celu zablokowania zapetlania sie sciezek
std::list<std::list<int>>
BranchAndBound::buildSubToursFromEdges(std::pair<int, int> edge, const list<std::list<int>> &subTours) {
    std::list<std::list<int>> subToursNew = subTours;
    // Jesli wezel drzewa nie posiada zadnych krawedzi sciezki
    if (subTours.empty()) {
        list<int> path;
        path.push_back(edge.first);
        path.push_back(edge.second);
        subToursNew.push_back(path);
        return subToursNew;
    }
    // Dodawanie wierzcholkow sciezki do listy sciezek
    bool isAdded = false;
    for (auto &item: subToursNew) {
        if (item.back() == edge.first) {
            item.push_back(edge.second);
            isAdded = true;
            break;
        }
        if (item.front() == edge.second) {
            item.push_front(edge.first);
            isAdded = true;
            break;
        }
    }
    if (!isAdded) {
        list<int> path;
        path.push_back(edge.first);
        path.push_back(edge.second);
        subToursNew.push_back(path);
    }
    // Proba polaczenia sciezek
    std::list<std::list<int>> result = subToursNew;
    std::list<std::list<int>> tmp = result;
    auto currentIt = tmp.begin();
    while (currentIt != tmp.end()) {
        auto currentTour = *currentIt;
        for (const auto &item: result) {
            if (item == currentTour) continue;
            if (item.front() == currentTour.back()) {
                for (const auto &v: item) {
                    if (v == item.front()) continue;
                    currentIt->push_back(v);
                }
                tmp.erase(std::remove(tmp.begin(), tmp.end(), item), tmp.end());
                break;
            }
            if (item.back() == currentTour.front()) {
                for (const auto &v: item) {
                    if (v == item.back()) continue;
                    currentIt->push_front(v);
                }
                tmp.erase(std::remove(tmp.begin(), tmp.end(), item), tmp.end());
                break;
            }
        }
        currentIt++;
        result = tmp;
    }
    return result;
}
