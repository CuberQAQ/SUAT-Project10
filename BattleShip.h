#ifndef _BATTLESHIP_H_
#define _BATTLESHIP_H_

#include "Ship.h"
#include "Obstacle.h"
#include "Weapon.h"
#include <vector>

class BattleShip : public Ship {
public:
    int coins;
    int shells;
    int missiles;
    int shellsFired;
    int missilesFired;
    int shellRegenTimer;
    int missileRegenTimer;

    BattleShip(int r, int c);
    void draw(Gui& gui) override;
    void update() override;
    void move(Direction d, int maxR, int maxC, const std::vector<Obstacle*>& obstacles);
    void shoot(std::vector<Weapon*>& weapons, bool triple, Direction forceDir = NONE);
    void launchMissile(std::vector<Weapon*>& weapons, GameObject* target);
};

#endif
