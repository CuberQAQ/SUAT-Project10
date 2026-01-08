#ifndef _GAME_H_
#define _GAME_H_

#include <vector>
#include <string>
#include "Gui.h"
#include "Types.h"
#include "GameObject.h"
#include "Obstacle.h"
#include "Weapon.h"
#include "Item.h"
#include "Ship.h"
#include "BattleShip.h"
#include "EnemyShip.h"
#include "GunBoat.h"
#include "Destroyer.h"
#include "Cruiser.h"
#include "Bomber.h"

class Game {
public:
    Game();
    ~Game();
    void run();
    void loadMap(const std::string& filename);

private:
    Gui gui;
    bool running;
    
    int rows;
    int cols;
    int level;
    int spawnTimer;
    
    BattleShip* player;
    std::vector<EnemyShip*> enemies;
    std::vector<Bomber*> bombers;
    std::vector<Weapon*> weapons;
    std::vector<Item*> items;
    std::vector<Obstacle*> obstacles;
    
    void handleInput();
    void update();
    void draw();
    void checkCollisions();
    void cleanup();
    void spawnEnemies();
    void nextLevel();
};

#endif
