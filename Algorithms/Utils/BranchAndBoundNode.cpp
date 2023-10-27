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

BranchAndBoundNode::BranchAndBoundNode(const BranchAndBoundNode &node) {
    data = node.data;
    size = node.size;
    location = node.location;
    parent = node.parent;
    branchedOut = node.branchedOut;
    remainingVertices = node.remainingVertices;
    upperBound = node.upperBound;
    path = node.path;
    with = node.with;
    without = node.without;
}

BranchAndBoundNode::BranchAndBoundNode(int **data, int size, int location, BranchAndBoundNode *parent, bool branchedOut,
                                       int remainingVertices, int upperBound, const vector<pair<int, int>> &path,
                                       const vector<pair<int, int>> &with, const vector<pair<int, int>> &without)
        : data(data), size(size), location(location), parent(parent), branchedOut(branchedOut),
          remainingVertices(remainingVertices), upperBound(upperBound), path(path), with(with), without(without) {
}

