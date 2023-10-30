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

BranchAndBoundNode::BranchAndBoundNode(BranchAndBoundNode &node) {
    data = node.data;
    size = node.size;
    location = node.location;
    upperBound = node.upperBound;
    with = node.with;
    without = node.without;
}
