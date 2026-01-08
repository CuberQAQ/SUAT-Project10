#include "Game.h"
#include <fstream>
#include <algorithm>
#include <cstring>
#include <cmath>
#ifndef _WIN32
#include <unistd.h> // for usleep
#endif
#include <thread>
#include <chrono>

Game::Game() : rows(31), cols(80), level(1), spawnTimer(0), running(false) {
    player = new BattleShip(rows/2, cols/2);
    // Add some random obstacles if map not loaded, or load map
}

Game::~Game() {
    delete player;
    for(auto e : enemies) delete e;
    for(auto b : bombers) delete b;
    for(auto w : weapons) delete w;
    for(auto i : items) delete i;
    for(auto o : obstacles) delete o;
}

void Game::loadMap(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Fallback: Generate border walls and random obstacles
        // Shift map down by 1 to reserve row 0 for UI
        for (int r = 1; r < rows; ++r) {
            obstacles.push_back(new Obstacle(r, 0));
            obstacles.push_back(new Obstacle(r, cols - 1));
        }
        for (int c = 1; c < cols - 1; ++c) {
            obstacles.push_back(new Obstacle(1, c)); // Top wall at row 1
            obstacles.push_back(new Obstacle(rows - 1, c));
        }
        // Random obstacles inside (Larger islands)
        for(int i=0; i<10; ++i) { // Fewer islands, but larger
            int r = rand() % (rows - 6) + 3; // Keep away from edges
            int c = rand() % (cols - 6) + 3;
            
            // Create a small island cluster (e.g., 2x2 or 3x3 or random shape)
            int size = rand() % 3 + 2; // Size 2 to 4
            for (int dr = 0; dr < size; ++dr) {
                for (int dc = 0; dc < size; ++dc) {
                    if (r+dr < rows-1 && c+dc < cols-1 && (rand()%10 > 2)) { // 80% chance to fill cell
                        obstacles.push_back(new Obstacle(r+dr, c+dc));
                    }
                }
            }
        }
        return;
    }
    
    char c;
    // Start reading map content into row 1
    int r = 1, col = 0;
    int maxR = 0;
    int maxC = 0;
    
    // First pass: determine map size
    std::ifstream fileDims(filename);
    if (fileDims.is_open()) {
        int tempR = 0;
        int tempC = 0;
        char tempCh;
        while (fileDims.get(tempCh)) {
            if (tempCh == '\n') {
                if (tempC > maxC) maxC = tempC;
                tempR++;
                tempC = 0;
            } else {
                tempC++;
            }
        }
        if (tempC > 0) tempR++; // Last line without newline
        if (tempC > maxC) maxC = tempC;
        maxR = tempR;
        
        // Update game dimensions if map is smaller than default
        // We keep minimum size to avoid issues, but update if map is valid
        if (maxR > 10 && maxC > 20) {
            rows = maxR + 1; // Add 1 row for UI
            cols = maxC;
        }
    }

    while (file.get(c)) {
        if (c == '\n') {
            r++;
            col = 0;
        } else {
            if (c == '#') {
                obstacles.push_back(new Obstacle(r, col));
            }
            col++;
        }
    }
}

void Game::run() {
    running = true;
    while(running) {
        handleInput();
        update();
        draw();
        
        if (player->health <= 0) {
            gui.clear();
            gui.printMsg(rows/2, cols/2 - 5, "GAME OVER");
            gui.redraw();
            gui.get(); // Wait for key
            running = false;
        }
        
        if (player->coins >= 1000) {
            nextLevel();
        }

        // Control frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::handleInput() {
    int ch = gui.get();
    if (ch == ERR) return;

    switch(ch) {
        case KEY_UP: player->move(UP, rows, cols, obstacles); break;
        case KEY_DOWN: player->move(DOWN, rows, cols, obstacles); break;
        case KEY_LEFT: player->move(LEFT, rows, cols, obstacles); break;
        case KEY_RIGHT: player->move(RIGHT, rows, cols, obstacles); break;
        case ' ': player->shoot(weapons, false); break;
        case 'a': {
             // Fire 3 bullets to relative LEFT
             Direction d = LEFT;
             switch(player->dir) {
                 case UP: d = LEFT; break;
                 case DOWN: d = RIGHT; break;
                 case LEFT: d = DOWN; break;
                 case RIGHT: d = UP; break;
                 default: break;
             }
             player->shoot(weapons, true, d); 
             break; 
        }
        case 'd': {
             // Fire 3 bullets to relative RIGHT
             Direction d = RIGHT;
             switch(player->dir) {
                 case UP: d = RIGHT; break;
                 case DOWN: d = LEFT; break;
                 case LEFT: d = UP; break;
                 case RIGHT: d = DOWN; break;
                 default: break;
             }
             player->shoot(weapons, true, d); 
             break;
        }
        case 'u': 
             // Requirement: "Fire shells/launch missiles ('u')"
             // Let's assume 'u' launches missile
             {
                 // Find nearest enemy
                 GameObject* target = nullptr;
                 if (!enemies.empty()) target = enemies[0]; 
                 player->launchMissile(weapons, target); 
             }
             break;
        case 'q': running = false; break;
    }
}

void Game::update() {
    spawnEnemies();

    // Update Player
    // (Player update mostly input, but maybe recharge logic)
    player->update();
    
    // Update Enemies
    for (auto e : enemies) {
        if (e->active) e->update(rows, cols, obstacles, player, weapons, enemies);
    }
    
    // Update Bombers
    for (auto b : bombers) {
        b->update(weapons);
    }
    
    // Update Weapons
    for (auto w : weapons) {
        w->update();
        
        // Missile homing logic
        if (w->type == MISSILE) {
            int targetR, targetC;
            if (w->isPlayerWeapon) {
                // Find nearest enemy
                if (!enemies.empty()) {
                    targetR = enemies[0]->row;
                    targetC = enemies[0]->col;
                } else {
                    targetR = -100; // No target
                }
            } else {
                targetR = player->row;
                targetC = player->col;
            }
            
            if (targetR != -100) {
                int dR = targetR - w->row;
                int dC = targetC - w->col;
                
                // Manhattan movement: only move along one axis per frame
                if (abs(dR) > abs(dC)) {
                    if (dR > 0) w->row++;
                    else w->row--;
                } else {
                    if (dC > 0) w->col++;
                    else if (dC < 0) w->col--; // Ensure we don't move if dC is 0
                }
            } else {
                // No target: Move in straight line (fallback)
                switch(w->dir) {
                    case UP: w->row--; break;
                    case DOWN: w->row++; break;
                    case LEFT: w->col--; break;
                    case RIGHT: w->col++; break;
                    default: break;
                }
            }
        }
    }
    
    checkCollisions();
    cleanup();
}

void Game::spawnEnemies() {
    // Increase difficulty: Spawn faster as level increases
    // Base 50, decrease by 5 per level, min 10
    int spawnRate = 50 - (level * 5);
    if (spawnRate < 10) spawnRate = 10;
    
    spawnTimer++;
    if (spawnTimer > spawnRate) { // Spawn rate depends on level
        spawnTimer = 0;
        int r = rand() % 100;
        
        int spawnR = rand() % rows;
        int spawnC = rand() % cols;
        bool safeSpawn = false;
        int attempts = 0;
        
        // Helper lambda to check if position is blocked
        auto isBlocked = [&](int r, int c, bool isCruiser) -> bool {
            for (auto o : obstacles) {
                if (isCruiser) {
                    if (o->row == r && (o->col >= c-1 && o->col <= c+1)) return true;
                } else {
                    if (o->row == r && o->col == c) return true;
                }
            }
            // Check against player too
            if (isCruiser) {
                 if (abs(player->row - r) <= 1 && abs(player->col - c) <= 2) return true;
            } else {
                 if (abs(player->row - r) <= 1 && abs(player->col - c) <= 1) return true;
            }
            return false;
        };

        if (r < 50) { // 50% chance Gunboat
            while (attempts < 10 && !safeSpawn) {
                // Stricter spawn area: [3, rows-4] and [3, cols-4]
                spawnR = rand() % (rows - 6) + 3; 
                spawnC = rand() % (cols - 6) + 3; 
                if (!isBlocked(spawnR, spawnC, false)) safeSpawn = true;
                attempts++;
            }
            if (safeSpawn) enemies.push_back(new GunBoat(spawnR, spawnC));
        } else if (r < 80) { // 30% chance Destroyer
            while (attempts < 10 && !safeSpawn) {
                spawnR = rand() % (rows - 6) + 3;
                spawnC = rand() % (cols - 6) + 3;
                if (!isBlocked(spawnR, spawnC, false)) safeSpawn = true;
                attempts++;
            }
            if (safeSpawn) enemies.push_back(new Destroyer(spawnR, spawnC));
        } else if (r < 90) { // 10% chance Cruiser
             // Ensure Cruiser spawns within strict bounds (2 to cols-3) to avoid edge issues
             while (attempts < 10 && !safeSpawn) {
                spawnR = rand() % (rows - 6) + 3;
                spawnC = rand() % (cols - 10) + 5; // Even stricter for Cruiser
                if (!isBlocked(spawnR, spawnC, true)) safeSpawn = true;
                attempts++;
             }
             if (safeSpawn) enemies.push_back(new Cruiser(spawnR, spawnC));
        } else { // 10% chance Bomber
             // Strictly restrict Bomber spawn to valid rows [3, rows-4] to avoid border overlaps
             int bomberR = rand() % (rows - 6) + 3;
             // Bombers start at col 0 or cols-1, which is fine as they fly in.
             // But we should ensure 'bomberR' is not on top/bottom wall.
             bombers.push_back(new Bomber(bomberR, rand()%2 == 0 ? 0 : cols-1, rand()%2==0 ? RIGHT : LEFT));
        }
        
        // Spawn Items
        if (rand() % 100 < 5) {
             int attempts = 0;
             while (attempts < 10) {
                 int iR = rand() % (rows - 6) + 3;
                 int iC = rand() % (cols - 6) + 3;
                 if (!isBlocked(iR, iC, false)) {
                     items.push_back(new Item(iR, iC, WEAPON_PACK));
                     break;
                 }
                 attempts++;
             }
        }
        if (rand() % 100 < 5) {
             int attempts = 0;
             while (attempts < 10) {
                 int iR = rand() % (rows - 6) + 3;
                 int iC = rand() % (cols - 6) + 3;
                 if (!isBlocked(iR, iC, false)) {
                     items.push_back(new Item(iR, iC, MEDICAL_PACK));
                     break;
                 }
                 attempts++;
             }
        }
    }
}

void Game::checkCollisions() {
    // Weapons vs Ships/Obstacles
    for (auto w : weapons) {
        if (!w->active) continue;
        
        // Bounds
        if (w->row < 0 || w->row >= rows || w->col < 0 || w->col >= cols) {
            w->active = false;
            continue;
        }
        
        // Obstacles
        bool hitObstacle = false;
        for (auto o : obstacles) {
            if (w->row == o->row && w->col == o->col) {
                hitObstacle = true;
                break;
            }
        }
        
        // Missiles can fly over islands (obstacles), but Shells/Torpedoes cannot
        if (hitObstacle && w->type != MISSILE) {
            w->active = false;
            continue;
        }
        
        // Player
        if (!w->isPlayerWeapon) {
            // Check collision with player body
            bool hit = false;
            // Simplified check: center point. For multi-tile ships, need better box check.
            if (abs(w->row - player->row) <= 1 && abs(w->col - player->col) <= 1) {
                 player->takeDamage(w->damage);
                 w->active = false;
            }
        }
        
        // Enemies
        if (w->isPlayerWeapon) {
            for (auto e : enemies) {
                if (!e->active) continue;
                if (abs(w->row - e->row) <= 1 && abs(w->col - e->col) <= 1) {
                    e->takeDamage(w->damage);
                    w->active = false;
                    if (!e->active) {
                        if (e->type == CRUISER) player->coins += 10;
                        else player->coins += 1;
                    }
                    break;
                }
            }
        }
    }
    
    // Player vs Items
    for (auto i : items) {
        if (abs(player->row - i->row) <= 1 && abs(player->col - i->col) <= 1) {
            if (i->type == WEAPON_PACK) {
                player->shells += 20;
                player->missiles += 1;
            } else {
                player->health += 100;
            }
            i->active = false;
        }
    }
}

void Game::cleanup() {
    // Remove inactive objects
    auto wIt = weapons.begin();
    while (wIt != weapons.end()) {
        if (!(*wIt)->active) {
            delete *wIt;
            wIt = weapons.erase(wIt);
        } else {
            ++wIt;
        }
    }
    
    auto eIt = enemies.begin();
    while (eIt != enemies.end()) {
        if (!(*eIt)->active) {
            delete *eIt;
            eIt = enemies.erase(eIt);
        } else {
            ++eIt;
        }
    }
    
    auto iIt = items.begin();
    while (iIt != items.end()) {
        if (!(*iIt)->active) {
            delete *iIt;
            iIt = items.erase(iIt);
        } else {
            ++iIt;
        }
    }
    
    auto bIt = bombers.begin();
    while (bIt != bombers.end()) {
        if (!(*bIt)->active) {
            delete *bIt;
            bIt = bombers.erase(bIt);
        } else {
            ++bIt;
        }
    }
}

void Game::draw() {
    gui.clear();
    
    // Draw Objects
    for (auto o : obstacles) o->draw(gui);
    for (auto i : items) i->draw(gui);
    for (auto e : enemies) e->draw(gui);
    for (auto b : bombers) b->draw(gui);
    player->draw(gui); // Player on top
    for (auto w : weapons) w->draw(gui);

    // Draw Stats (Draw LAST to ensure visibility on top of map/objects)
    // Clear the line first to ensure readability
    for(int c=0; c<cols; c++) gui.paintat(0, c, ' ', 0);
    
    int x = 0;
    // Health
    gui.printMsg(0, x, "Health:"); x += 7;
    char valBuf[32];
    sprintf(valBuf, " %d ", player->health);
    for(int i=0; valBuf[i]; i++) gui.paintat(0, x+i, valBuf[i], COL_PLAYER);
    x += strlen(valBuf) + 2;

    // Coins
    gui.printMsg(0, x, "Coins:"); x += 6;
    sprintf(valBuf, " %d ", player->coins);
    for(int i=0; valBuf[i]; i++) gui.paintat(0, x+i, valBuf[i], COL_ITEM);
    x += strlen(valBuf) + 2;

    // Shells
    gui.printMsg(0, x, "Shells:"); x += 7;
    sprintf(valBuf, " %d ", player->shells);
    for(int i=0; valBuf[i]; i++) gui.paintat(0, x+i, valBuf[i], COL_DEFAULT);
    x += strlen(valBuf) + 2;

    // Missiles
    gui.printMsg(0, x, "Missiles:"); x += 9;
    sprintf(valBuf, " %d ", player->missiles);
    for(int i=0; valBuf[i]; i++) gui.paintat(0, x+i, valBuf[i], COL_MISSILE);
    x += strlen(valBuf) + 2;

    // Level
    gui.printMsg(0, x, "Level:"); x += 6;
    sprintf(valBuf, " %d ", level);
    for(int i=0; valBuf[i]; i++) gui.paintat(0, x+i, valBuf[i], COL_BOMBER); // Use Cyan for level
    
    gui.redraw();
}

void Game::nextLevel() {
    level++;
    player->coins = 0;
    // Difficulty increase?
    spawnTimer = -100; // Delay spawns
    gui.printMsg(rows/2, cols/2 - 5, "LEVEL UP!");
    gui.redraw();
    #ifdef _WIN32
    _sleep(2000);
    #else
    sleep(2);
    #endif
}
