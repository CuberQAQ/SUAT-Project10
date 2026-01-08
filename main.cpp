#include "Game.h"
#include <ctime>
#include <cstdlib>

int main() {
    srand(time(NULL));
    Game game;
    game.loadMap("map.txt");
    game.run();
    return 0;
}
