#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include <iostream>

#include "Board.hpp"
#include "MoveNode.hpp"

using namespace std;

/**
 * Store the board to play and can play it for the user or play it to find the theoretical move count to complete the game.
 */
class Game {

public:
    /**
     * @param board The board that you want to play with.
     */
    explicit Game(Board *board);

    /**
     * Run the game accepting user color input with each turn while printing the board at each stage.
     */
    void runForUser();

    /**
     * What the least number of moves to complete the game is
     *
     * @return Minimum number of steps to complete the game
     */
    std::vector<MoveNode *> runToFindTheoreticalMinStepsForCompletion();

    virtual ~Game();

private:
    Board *board;
    MoveNode *moveGraph;
    unordered_map<string, int> cachedBoards; // cache board contents with the least number of moves to get there

    /**
     * The number of moves to move from one state of the board to another.
     *
     * @param node A board state before succ
     * @param succ A board state further on in the game
     */
    static int cost(MoveNode *node, MoveNode *succ);

    /**
     * Can the game be be completed to a certain depth
     *
     * @param path Path of moves to a board state
     * @param g The cost to reach current node
     * @param bound The depth of deepening
     * @return The num of moves to complete the game or -1 if not found
     */
    int search(vector<MoveNode *> *path, int g, int bound);

    /**
     * Run iterative deepening algorithm a* search
     *
     * @param root The start node at the top of the tree. Should not have performed a move.
     * @return The num of moves to complete the game or -1 if not found (this should not happen if correct parameters)
     */
    std::vector<MoveNode *> ida_star(MoveNode *root);
};

#endif /* GAME_H */
