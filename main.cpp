#include "Game.h"
#include <ctime>
#include <cstdlib>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    Game game;
    if (argc > 1) {
        game.loadMap(argv[1]);
    } else {
        game.loadMap("");
    }
    game.run();
    return 0;
}
