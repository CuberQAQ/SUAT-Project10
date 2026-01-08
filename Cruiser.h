#ifndef _CRUISER_H_
#define _CRUISER_H_

#include "EnemyShip.h"

class Cruiser : public EnemyShip {
public:
    Cruiser(int r, int c);
    void draw(Gui& gui) override;
    void update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) override;
};

#endif