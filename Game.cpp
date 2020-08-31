#include <iostream>
#include <bits/stdc++.h>

#include "Board.hpp"
#include "Game.hpp"

using namespace std;

Game::Game(Board *board) {
    this->board = board;
    moveGraph = new MoveNode(-1, new Board(board->getNodes(), board->getSize()), 0,
                             6); // start node, no moves have been played
}

void Game::runForUser() {
    int moveNum = 1;
    MoveNode *currentMoveNode = moveGraph;

    while (true) {
        cout << "Move " << moveNum << ": " << endl;

        // user choose next color
        int colorCode;
        cout << "Input a color by number (1-" + to_string(Board::COLOR_RANGE) + "): "; // type a number and press enter
        while (!(cin >> colorCode) or !(1 <= colorCode and colorCode <= Board::COLOR_RANGE)) {
            cout << "Error: please enter a integer between 1-" + to_string(Board::COLOR_RANGE) + ": ";

            cin.clear();
            cin.ignore(123, '\n');
        }

        float nodesFilled = board->fillFromPivot(colorCode);
        cout << *board << endl;

        // cache the player's path in the moves tree along with scores for future use
        currentMoveNode = currentMoveNode->getInterconnectedNodes()[colorCode - 1];

        if (nodesFilled == 1) {
            cout << "Game completed in " << moveNum << " moves" << endl;
            break;
        }

        moveNum++;
    }

}

int Game::cost(MoveNode *node, MoveNode *succ) {
    return succ->numMovesToGetHere - node->numMovesToGetHere;
}

/**
 * Uses the a* star search algorithm to depth of parameter bound
 */
int Game::search(std::vector<MoveNode *> *path, int g, int bound) {
    MoveNode *node = path->back();
    int f = g + node->getCost();
    if (f > bound) {
        return f;
    }
    if (node->isSolution) return -1;

    int min = std::numeric_limits<int>::max();
    for (MoveNode *succ : node->getInterconnectedNodes()) {
        /* Optimisation - if the exact same board had been visited before, had it been reached in less moves?
         * If so then stop going down the current path because will it follow the same path as before but having
         * performed more moves.
         */
        string nodeInt = succ->tempBoard->toInt();
        if (cachedBoards.find(nodeInt) != cachedBoards.end()) {
            if (succ->numMovesToGetHere > cachedBoards[nodeInt]) {
                continue;
            }
        } else {
            cachedBoards[nodeInt] = succ->numMovesToGetHere;
        }

        if (std::find(std::begin(*path), std::end(*path), succ) == std::end(*path)) { // if succ not in path
            path->push_back(succ);
            int t = search(path, g + cost(node, succ), bound);
            if (t == -1) return -1;
            if (t < min) min = t;
            path->pop_back();
        }
    }

    return min;
}

/**
 * Uses iterative deepening a* search to search for solutions to the game.
 * Bound initially the starting board state cost minimum move estimation then is changed to new minimum estimations when
 * travelling further down the tree.
 * Using heuristic: number of colors left on the board
 */
std::vector<MoveNode *> Game::ida_star(MoveNode *root) {
    int bound = root->getCost();
    std::vector<MoveNode *> path = {root};
    while (true) {
        int t = search(&path, 0, bound);

        if (t == -1) {
            path.erase(path.begin()); // remove root node
            return path;
        }
        if (t == std::numeric_limits<int>::max()) return {};

        bound = t;
    }
}

std::vector<MoveNode *> Game::runToFindTheoreticalMinStepsForCompletion() {
    return ida_star(moveGraph);
}

/*
 * Recursively frees each MoveNode in the moveGraph tree
 */
void deleteMoveGraph(MoveNode *head) {
    if (head == nullptr)
        return;

    for (MoveNode *moveNode : head->getRawInterconnectedNodes()) {
        deleteMoveGraph(moveNode);
    }

    delete head;
}

Game::~Game() {
    deleteMoveGraph(moveGraph);
}
