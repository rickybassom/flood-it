#include <iostream>
#include "Board.hpp"
#include "Game.hpp"

using namespace std;

int main(int argc, char **argv) {
    Board *board;

    if (argc > 1) {
        board = new Board(argv[1]);
    } else {
        int N;
        cout << "Input N for board: "; // Type a number and press enter
        while (!(cin >> N)) {
            cout << "Error: please enter an integer";

            cin.clear();
            cin.ignore(123, '\n');
        }

        board = new Board(N);
    }

    cout << "Loaded board: " << endl;
    cout << *board << "\n";

    auto *game = new Game(board);
    game->runForUser();

    std::vector<MoveNode *> path = game->runToFindTheoreticalMinStepsForCompletion();
    cout << "Best theoretical completion move count is: " << path.size()
         << endl;

    cout << "An optimal move set: ";
    for(MoveNode *moveNode : path) {
        cout << moveNode->colorCode << " ";
    }
    cout << "\n" << endl;

    delete game;
    delete board;

    return 0;
}
