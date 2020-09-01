#include <vector>
#include "Node.hpp"

Node::Node(int value) : value(value) {}

std::vector<Node *> Node::getInterconnectedNodes() {
    return {right, left, down, up};
}