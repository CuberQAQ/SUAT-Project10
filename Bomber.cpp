#include "Bomber.h"
#include <cstdlib>

Bomber::Bomber(int r, int c, Direction d) : GameObject(r, c, COL_BOMBER), flyDir(d), dropTimer(0) {}

void Bomber::draw(Gui& gui) {
    // /
    // ==
    // \ 
    // or reverse
    if (flyDir == RIGHT) {
        gui.paintat(row-1, col, '\\', color);
        gui.paintat(row, col, '=', color);
        gui.paintat(row, col-1, '=', color);
        gui.paintat(row+1, col, '/', color);
    } else {
        gui.paintat(row-1, col, '/', color);
        gui.paintat(row, col, '=', color);
        gui.paintat(row, col+1, '=', color);
        gui.paintat(row+1, col, '\\', color);
    }
}

void Bomber::update(std::vector<Weapon*>& weapons) {
    if (flyDir == RIGHT) col++; else col--;
    if (col < -2 || col > 82) active = false; // Out of bounds

    dropTimer++;
    if (dropTimer > 5) {
        dropTimer = 0;
        // Requirement: "Can shoot three bullets at one time, has 3 bullets or drop a torpedo"
        int r = rand() % 2;
        if (r == 0) {
            // Drop Torpedo
            weapons.push_back(new Weapon(row, col, TORPEDO, DOWN, false));
        } else {
            // Shoot 3 bullets (Spread downwards)
            weapons.push_back(new Weapon(row, col, SHELL, DOWN, false));
            weapons.push_back(new Weapon(row, col, SHELL, LEFT, false)); // Or angled down-left if we had diagonals
            weapons.push_back(new Weapon(row, col, SHELL, RIGHT, false));
        }
    }
}
