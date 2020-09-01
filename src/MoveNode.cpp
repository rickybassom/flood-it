#include <vector>
#include "MoveNode.hpp"

MoveNode::MoveNode(int colorCode, Board *tempBoard, int numMovesToGetHere, int numColors) : colorCode(colorCode),
                                                                                            tempBoard(tempBoard),
                                                                                            numMovesToGetHere(
                                                                                                    numMovesToGetHere),
                                                                                            numColors(numColors) {}

/**
 * Returns nodes from numColours available in the game. Caches nodes if created before.
 */
std::vector<MoveNode *> MoveNode::getInterconnectedNodes() {
    if (interconnectedNodes.empty()) {
        interconnectedNodes.reserve(numColors);
        for (int x = 1; x <= numColors; x++) {
            auto newTempBoard = new Board(tempBoard->getNodes(), tempBoard->getSize());
            float completed = newTempBoard->fillFromPivot(x);
            auto newMove = new MoveNode(x, newTempBoard, numMovesToGetHere + 1, numColors);
            if (completed == 1) newMove->isSolution = true;
            interconnectedNodes.push_back(newMove);
        }

        return interconnectedNodes;
    } else {
        return interconnectedNodes;
    }

}

std::vector<MoveNode *> MoveNode::getRawInterconnectedNodes() {
    return interconnectedNodes;
}

int MoveNode::getCost() const {
    return tempBoard->getNumOfColorsOnBoard() - 1;
}

MoveNode::~MoveNode() {
    delete tempBoard;
}
