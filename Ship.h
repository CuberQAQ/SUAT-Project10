#ifndef _SHIP_H_
#define _SHIP_H_

#include "GameObject.h"

class Ship : public GameObject {
public:
    int health;
    int maxHealth;
    Direction dir;
    int moveTimer;
    int moveInterval; // Slower movement

    Ship(int r, int c, int hp, int clr);
    virtual void takeDamage(int dmg);
};

#endif
