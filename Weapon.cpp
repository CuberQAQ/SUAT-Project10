#include "Weapon.h"

Weapon::Weapon(int r, int c, WeaponType t, Direction d, bool isPlayer) 
    : GameObject(r, c, isPlayer ? COL_PLAYER : (t == MISSILE ? COL_MISSILE : COL_ENEMY)), 
      type(t), dir(d), isPlayerWeapon(isPlayer) {
    if (type == SHELL) damage = 1;
    else if (type == TORPEDO) damage = 10;
    else if (type == MISSILE) damage = 5;
}

void Weapon::draw(Gui& gui) {
    char sym = '-';
    if (type == SHELL) {
        sym = (dir == UP || dir == DOWN) ? '|' : '-';
    } else if (type == TORPEDO) {
        sym = '=';
    } else if (type == MISSILE) {
        sym = '*';
    }
    gui.paintat(row, col, sym, color);
}

void Weapon::update() {
    // Missile movement is handled entirely in Game::update (homing or fallback)
    if (type == MISSILE) return;

    // Basic movement
    switch(dir) {
        case UP: row--; break;
        case DOWN: row++; break;
        case LEFT: col--; break;
        case RIGHT: col++; break;
        default: break;
    }
}
