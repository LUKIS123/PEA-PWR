#include "BranchAndBoundNode.h"

BranchAndBoundNode::~BranchAndBoundNode() {
    for (int i = 0; i < size; i++) {
        delete[] data[i];
    }
    delete[] data;
}

BranchAndBoundNode::BranchAndBoundNode(int **data, int rem, int size, const vector<bool> &visited)
        : data(data), rem(rem), size(size), visited(visited) {

}
