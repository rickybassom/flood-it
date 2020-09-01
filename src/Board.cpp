#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stack>
#include <algorithm>
#include <bits/stdc++.h>

#include "Board.hpp"
#include "Node.hpp"

using namespace std;


Board::Board(const string &fileName) {
    ifstream ist;
    ist.open(fileName);

    if (!ist) {
        fprintf(stderr, "Can't open input file %s\n", fileName.c_str());
        exit(EXIT_FAILURE);
    }

    string firstChar;
    ist >> firstChar;
    this->size = stoi(firstChar);

    if (!isGridInBounds(this->size)) {
        fprintf(stderr, "Grid size out of bounds N (%d ≤ N ≤ %d)\n", MIN_GRID_BOUND, MAX_GRID_BOUND);
        exit(EXIT_FAILURE);
    }

    // read file into nodes 2d array
    string temp;
    nodes = new Node **[this->size];
    int numberOfNodes = 0;

    for (int x = 0; x < this->size; x++) {
        nodes[x] = new Node *[this->size];

        for (int y = 0; y < this->size; y++) {
            if (!(ist >> temp)) {
                ist.close();
                fprintf(stderr, "Grid size does not match number of nodes\n");
                exit(EXIT_FAILURE);
            }
            nodes[x][y] = new Node(stoi(temp));
            numberOfNodes++;
        }
    }

    if (ist >> temp) {
        fprintf(stderr, "Grid size does not match number of nodes\n");
        exit(EXIT_FAILURE);
    }

    ist.close();

    pivot = nodes[0][0];
    generateNodes();
}

Board::Board(const int &N) {
    if (!isGridInBounds(N)) {
        fprintf(stderr, "Grid size out of bounds N (%d ≤ N ≤ %d)\n", MIN_GRID_BOUND, MAX_GRID_BOUND);
        exit(EXIT_FAILURE);
    }

    this->size = N;

    srand(time(nullptr));

    // randomly generate colors for nodes grid
    this->nodes = new Node **[N];
    for (int x = 0; x < N; x++) {
        this->nodes[x] = new Node *[N];

        for (int y = 0; y < N; y++) {
            this->nodes[x][y] = new Node(rand() % COLOR_RANGE + 1);
        }

    }

    pivot = nodes[0][0];
    generateNodes();
}

Board::Board(Node ***nodes, unsigned long size) {
    if (!isGridInBounds(size)) {
        std::fprintf(stderr, "Grid size out of bounds N (%d ≤ N ≤ %d)\n", MIN_GRID_BOUND, MAX_GRID_BOUND);
        exit(EXIT_FAILURE);
    }

    this->size = size;

    // deep copy nodes to another 2d array
    this->nodes = new Node **[size];

    for (int x = 0; x < size; x++) {
        this->nodes[x] = new Node *[size];

        for (int y = 0; y < size; y++) {
            Node *newNode = new Node(*nodes[x][y]);
            this->nodes[x][y] = newNode;
        }

    }

    pivot = this->nodes[0][0];
    generateNodes();
}

unsigned long Board::getSize() const {
    return this->size;
}

Node ***Board::getNodes() {
    return this->nodes;
}

void Board::generateNodes() {
    unordered_set<int> colorsUsed;
    unsigned long boardSize = this->size;

    for (int y = 0; y < boardSize; y++) {
        for (int x = 0; x < boardSize; x++) {
            if (x != 0) nodes[y][x]->left = nodes[y][x - 1];
            if (x != boardSize - 1) nodes[y][x]->right = nodes[y][x + 1];
            if (y != 0) nodes[y][x]->up = nodes[y - 1][x];
            if (y != boardSize - 1) nodes[y][x]->down = nodes[y + 1][x];

            colorsUsed.insert(nodes[x][y]->value);
        }
    }

    numColorsOnBoard = colorsUsed.size();
}

/**
 * Depth first traversal coloring from pivot to when color changes from pivot node color. DFS is better than 2d array
 * looping as it will only visit the nodes that should be coloured in.
 * Continues traversal after that to when color is not the same as new color inputted by the user to see if the whole
 * grid is one color. The is better than looping through the 2d array after the traversal to check for a single colour
 * as it requires less steps.
 */
float Board::fillFromPivot(int colorCode) {
    stack<Node *> stack;
    unordered_set<Node *> visited;

    unordered_set<Node *> stopColoringNode;
    int intialPivotColor = pivot->value;

    stack.push(pivot);

    if (pivot->value != colorCode) {
        numColorsOnBoard--;
    }

    while (!stack.empty()) {
        Node *vertex = stack.top();
        stack.pop();

        visited.insert(vertex);

        if (colorCode != vertex->value) {
            vertex->value = colorCode;
        }

        for (Node *connectedNode : vertex->getInterconnectedNodes()) {
            if (connectedNode != nullptr and visited.find(connectedNode) == visited.end()) {
                if (connectedNode->value == intialPivotColor
                    and stopColoringNode.find(connectedNode) == stopColoringNode.end()
                    and stopColoringNode.find(vertex) == stopColoringNode.end()) { // color nodes
                    stack.push(connectedNode);
                } else if (connectedNode->value == colorCode) { // after visit nodes of the same color as colorCode
                    stopColoringNode.insert(connectedNode);
                    stack.push(connectedNode);
                }
            }
        }
    }

    return (float) visited.size() / (float) (this->getSize() * this->getSize()); // 0.0 to 1.0 of how much of board full
}

bool Board::isGridInBounds(const unsigned long &N) {
    return !(N < MIN_GRID_BOUND or N > MAX_GRID_BOUND);

}

int Board::getNumOfColorsOnBoard() const {
    return numColorsOnBoard;
}

string Board::toInt() {
    string string;
    for (int x = 0; x < this->size; x++) {
        for (int y = 0; y < this->size; y++) {
            string.append(to_string(this->nodes[x][y]->value));
        }

    }
    return string;
}

std::ostream &operator<<(std::ostream &strm, const Board &v) {
    for (int x = 0; x < v.size; x++) {
        for (int y = 0; y < v.size; y++) {
            strm << "\033[" << v.nodeColorCodes[v.nodes[x][y]->value - 1] << "m"
                 << v.nodes[x][y]->value << " \033[0m";
        }

        strm << endl;
    }

    return strm;
}

Board::~Board() {
    //nodes free each sub-array
    for (int x = 0; x < getSize(); x++) {
        for (int y = 0; y < getSize(); y++) {
            delete nodes[x][y];
        }

        delete[] nodes[x];
    }

    //nodes free the array of pointers
    delete[] nodes;
}
