#ifndef PEA_PWR_BRANCHANDBOUNDNODE_H
#define PEA_PWR_BRANCHANDBOUNDNODE_H

#include <iostream>
#include <list>

using namespace std;

// Obiekt wezla poddrzewa posiada takie atrybuty jak
// macierz poddrzewa, rozmiar macierzy, ograniczenie, liste krawedzi ktore zawiera, liste krawedzi odrzucownych, liste podsciezek
class BranchAndBoundNode {
public:
    int **data;
    int size;

    // Dolne ograniczenie
    int lowerBound = 0;
    // Lista krawedzi ktore zawiera wezel drzewa
    list<pair<int, int>> with;
    // Lista podsciezek
    list<list<int>> subTours;

    BranchAndBoundNode(int **data, int size);

    BranchAndBoundNode(BranchAndBoundNode &node);

    virtual ~BranchAndBoundNode();

    bool operator<(const BranchAndBoundNode &other) const {
        return lowerBound < other.lowerBound;
    }

    bool operator>(const BranchAndBoundNode &other) const {
        return lowerBound > other.lowerBound;
    }

    class comp {
    public:
        bool operator()(const BranchAndBoundNode *lhs, const BranchAndBoundNode *rhs) const {
            return lhs->lowerBound > rhs->lowerBound;
        }
    };
};


#endif //PEA_PWR_BRANCHANDBOUNDNODE_H
