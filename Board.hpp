#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <vector>
#include "Node.hpp"

using namespace std;

/**
 * Stores nodes, properties and can flood fill from pivot
 */
class Board {
public:
    /**
     * Create board of size NxN
     * @param N The size of the Board, create NxN nodes.
     */
    explicit Board(const int &N);

    /**
     * Reads from file and creates a board object from it
     * @param fileName
     */
    explicit Board(const std::string &fileName);

    /**
     * Create board from a 2d array of Nodes
     *
     * @param nodes Linked list of Node
     * @param size Size of 2d array size*size
     */
    explicit Board(Node ***nodes, unsigned long size);

    static const int MIN_GRID_BOUND = 3;
    static const int MAX_GRID_BOUND = 9;
    static const int COLOR_RANGE = 6;

    /**
     * @return size where number of nodes = size*size
     */
    unsigned long getSize() const;

    /**
     * @return 2d array of nodes
     */
    Node ***getNodes();

    /**
     * Flood fill the board from pivot with color
     *
     * @param colorCode
     * @return float between 0-1 of how much of the board has been filled by the user
     */
    float fillFromPivot(int colorCode);

    /**
     * @return The number of colors left on the board
     */
    int getNumOfColorsOnBoard() const;

    /**
     * @return the board as a number unique to the board node formation. e.g 123123 for a 3x3 grid
     */
    string toInt();

    friend std::ostream &operator<<(ostream &strm, Board const &v);

    virtual ~Board();

private:
    const string nodeColorCodes[COLOR_RANGE] = { // for terminal color output
            "41", // red
            "42", // green
            "43", // yellow
            "44", // blue
            "45", // magenta
            "46" // cyan
    };

    unsigned long size; // board size where num of nodes = size*size
    Node *pivot; // the pivot is the top left node on the grid
    Node ***nodes; // a 2d array of Nodes
    int numColorsOnBoard{}; // number of colors remaining on the board

    void generateNodes(); // create a graph of nodes (left, right, up, down) from this->nodes

    static bool isGridInBounds(const unsigned long &N); // check to see if N is within MIN_GRID_BOUND and MAX_GRID_BOUND
};


#endif /* BOARD_H */
