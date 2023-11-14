#include <algorithm>
#include "BranchAndBoundNode.h"

using namespace std;

BranchAndBoundNode::~BranchAndBoundNode() {
    for (int i = 0; i < size; i++) {
        delete[] data[i];
    }
    delete[] data;
}

BranchAndBoundNode::BranchAndBoundNode(int **data, int size)
        : data(data), size(size) {
}

// Konstruktor kopiujacy
BranchAndBoundNode::BranchAndBoundNode(BranchAndBoundNode &node) {
    data = node.data;
    size = node.size;
    lowerBound = node.lowerBound;
    with = node.with;
    without = node.without;
    subTours = node.subTours;
}
