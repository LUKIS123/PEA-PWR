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

void BranchAndBoundNode::sortStackRecursive(stack<BranchAndBoundNode *> &s) {
    if (s.empty())
        return;

    BranchAndBoundNode *x = s.top();
    s.pop();

    sortStackRecursive(s);

    stack<BranchAndBoundNode *> tempStack;

    while (!s.empty() && s.top() > x) {
        tempStack.push(s.top());
        s.pop();
    }

    s.push(x);

    while (!tempStack.empty()) {
        s.push(tempStack.top());
        tempStack.pop();
    }
}

void BranchAndBoundNode::sort_stack_cmp(std::stack<BranchAndBoundNode *> &stack) {
    std::vector<BranchAndBoundNode *> tmp_container;
    tmp_container.reserve(stack.size());
    while (!stack.empty()) {
        tmp_container.push_back(std::move(stack.top()));
        stack.pop();
    }
    std::sort(tmp_container.begin(), tmp_container.end(), BranchAndBoundNode::comp());
    for (auto it: tmp_container) {
        stack.push(std::move(it));
    }
}
