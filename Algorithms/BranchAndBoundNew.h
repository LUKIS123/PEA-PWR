#ifndef PEA_PWR_BRANCHANDBOUNDNEW_H
#define PEA_PWR_BRANCHANDBOUNDNEW_H


#include "iostream"
#include "algorithm"
//#include <bits/stdc++.h>
#include "vector"
#include <queue>
#include "./Utils/Node.h"
#include "../Matrix/Matrix.h"

class BranchAndBoundNew {
public:
#define INF INT_MAX

    int **inputMatrix = nullptr;
    int matrixSize = 0;
    int **reducedMatrix = nullptr;


    void printPath(const std::vector<std::pair<int, int>> &list);

    int cost_calculation(int **matrix_reduced, int size);

    void reduce_column(int **matrix_reduced, int *col, int size);

    void reduce_row(int **matrix_reduced, int *row, int size);

    int mainFun(Matrix *matrix, int matrixSize);

    int solve(int **adjacencyMatrix);
};


#endif //PEA_PWR_BRANCHANDBOUNDNEW_H
