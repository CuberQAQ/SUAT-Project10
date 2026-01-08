#ifndef _DESTROYER_H_
#define _DESTROYER_H_

#include "EnemyShip.h"

class Destroyer : public EnemyShip {
public:
    Destroyer(int r, int c);
    void draw(Gui& gui) override;
    void update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) override;
};

#endif