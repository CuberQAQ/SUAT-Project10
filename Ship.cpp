#include "Ship.h"

Ship::Ship(int r, int c, int hp, int clr) : GameObject(r, c, clr), health(hp), maxHealth(hp), dir(UP), moveTimer(0), moveInterval(3) {}

void Ship::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) active = false;
}
