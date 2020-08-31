#ifndef NODE_H
#define NODE_H


/**
 * Node to form a graph for the board. Points to left, right, down, up on grid. Points to null if off grid.
 */
class Node {
public:
    explicit Node(int value);

    int value;

    Node *right = nullptr;
    Node *left = nullptr;
    Node *down = nullptr;
    Node *up = nullptr;

    /**
     * @return {right, left, down, up} pointers
     */
    std::vector<Node *> getInterconnectedNodes();

};


#endif /* NODE_H */
