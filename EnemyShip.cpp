#include "EnemyShip.h"

EnemyShip::EnemyShip(int r, int c, EnemyType t, int hp, int clr) : Ship(r, c, hp, clr), type(t), shootTimer(0), shootInterval(8), shells(0), torpedoes(0), missiles(0) {
    // Stats initialization moved to subclasses
}
