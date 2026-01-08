#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "GameObject.h"

class Weapon : public GameObject {
public:
    WeaponType type;
    Direction dir;
    bool isPlayerWeapon;
    int damage;

    Weapon(int r, int c, WeaponType t, Direction d, bool isPlayer);
    void draw(Gui& gui) override;
    void update() override;
};

#endif
