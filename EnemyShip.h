#ifndef _ENEMYSHIP_H_
#define _ENEMYSHIP_H_

#include "Ship.h"
#include "Obstacle.h"
#include "Weapon.h"
#include "BattleShip.h"
#include <vector>

class EnemyShip : public Ship {
public:
    EnemyType type;
    int shootTimer;
    int shootInterval;
    int shells;
    int torpedoes;
    int missiles;

    EnemyShip(int r, int c, EnemyType t, int hp, int clr);
    virtual ~EnemyShip() {}
    
    // Abstract update method to be implemented by subclasses, 
    // but we can also keep the common signature if we want strict polymorphism
    virtual void update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) = 0;
};

#endif
