#ifndef PEA_PWR_NODE_H
#define PEA_PWR_NODE_H

#include "iostream"
#include "algorithm"
#include "vector"

class Node {
#define INF INT_MAX

public:
    std::vector<std::pair<int, int>> path;
    int **matrix_reduced;
    int size;
    int cost;
    int vertex;
    int level;

    static Node *
    newNode(int **matrix_parent, int size, std::vector<std::pair<int, int>> const &path, int level, int i, int j);

    class comp {
    public:
        bool operator()(const Node *lhs, const Node *rhs) const {
            return lhs->cost > rhs->cost;
        }
    };

};


#endif //PEA_PWR_NODE_H
