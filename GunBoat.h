#ifndef _GUNBOAT_H_
#define _GUNBOAT_H_

#include "EnemyShip.h"

class GunBoat : public EnemyShip {
public:
    GunBoat(int r, int c);
    void draw(Gui& gui) override;
    void update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) override;
};

#endif