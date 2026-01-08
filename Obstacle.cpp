#include "Obstacle.h"

Obstacle::Obstacle(int r, int c) : GameObject(r, c, COL_OBSTACLE) {}

void Obstacle::draw(Gui& gui) {
    gui.paintat(row, col, '#', color);
}
