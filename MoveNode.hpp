#ifndef MOVENODE_H
#define MOVENODE_H

#include <bits/stdc++.h>
#include "Board.hpp"

/**
 * Node to form a graph for the a representation of the moves played by a user.
 * Points to the next possible moves a player can make.
 */
class MoveNode {
public:
    /*
     * @param colorCode The color played
     * @param tempBoard A snapshot of the board at that time in play
     * @param numMovesToGetHere The number of moves played to get to that move
     * @param numColors Number of colors that can be played
     */
    explicit MoveNode(int colorCode, Board *tempBoard, int numMovesToGetHere, int numColors);

    const int colorCode;
    Board *tempBoard;
    const int numMovesToGetHere;
    bool isSolution = false;

    const int numColors;

    /**
     * @return Generates then returns to next possible moves that can be made.
     */
    std::vector<MoveNode *> getInterconnectedNodes();

    /**
     * @return Returns empty array if new moves not created before using getInterconnectedNodes. Doesn't auto-generate.
     */
    std::vector<MoveNode *> getRawInterconnectedNodes();

    /**
     * @return the cost of the node used in a* traversal
     */
    int getCost() const;

    virtual ~MoveNode();

private:
    std::vector<MoveNode *> interconnectedNodes{};
};


#endif //MOVENODE_H
