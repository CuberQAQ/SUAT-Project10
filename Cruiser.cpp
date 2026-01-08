#include "Cruiser.h"
#include <cstdlib>
#include <cmath>

Cruiser::Cruiser(int r, int c) : EnemyShip(r, c, CRUISER, 100, 6) { // 100 HP, MAGENTA (6)
    shells = 30;
    missiles = 2;
    shootInterval = 15;
}

void Cruiser::draw(Gui& gui) {
    gui.paintat(row, col-1, '<', color);
    gui.paintat(row, col+1, '>', color);
}

void Cruiser::update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) {
    // Movement (Horizontal only)
    moveTimer++;
    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        Direction d = NONE;
        bool useAI = (rand() % 100 < 90) && player;
        
        if (useAI) {
            int dCol = player->col - col;
            if (dCol > 0) d = RIGHT;
            else if (dCol < 0) d = LEFT;
            else d = (rand() % 2 == 0) ? LEFT : RIGHT;
        } else {
            int rDir = rand() % 4;
            d = (Direction)rDir;
        }

        if (d == UP || d == DOWN) {
             // Ignore vertical
        } else {
            int nextR = row;
            int nextC = col;
            switch(d) {
                case LEFT: nextC--; break;
                case RIGHT: nextC++; break;
                default: break;
            }
            bool safe = false;
            // Stricter bounds for Cruiser
            if (nextR >= 0 && nextR < maxR && nextC >= 2 && nextC < maxC - 2) safe = true;

            if (safe) {
                for (auto obs : obstacles) {
                    if (obs->row == nextR && (obs->col == nextC - 1 || obs->col == nextC || obs->col == nextC + 1)) {
                        safe = false;
                        break;
                    }
                }
                if (safe && player) {
                    if (abs(player->row - nextR) <= 1 && abs(player->col - nextC) <= 2) safe = false;
                }
                if (safe) {
                    for (auto other : enemies) {
                        if (other == this || !other->active) continue;
                        if (other->type == CRUISER) {
                            if (abs(other->row - nextR) == 0 && abs(other->col - nextC) < 3) safe = false;
                        } else {
                            if (other->row == nextR && (nextC >= other->col - 1 && nextC <= other->col + 1)) safe = false;
                        }
                        if (!safe) break;
                    }
                }
            }

            if (safe) {
                 dir = d;
                 row = nextR;
                 col = nextC;
            }
        }
        
        // AI Aiming (Cruiser doesn't rotate visually but sets dir for... logic?)
        // Actually Cruiser fires 4 ways, so dir is less critical, but let's keep it facing player horizontally
        if (useAI && player) {
             int dCol = player->col - col;
             dir = (dCol > 0) ? RIGHT : LEFT;
        }
    }

    // Shooting
    shootTimer++;
    if (shootTimer >= shootInterval) {
        shootTimer = 0;
        // Cruiser always fires
        if (shells > 0) {
            weapons.push_back(new Weapon(row, col, SHELL, UP, false));
            weapons.push_back(new Weapon(row, col, SHELL, DOWN, false));
            weapons.push_back(new Weapon(row, col, SHELL, LEFT, false));
            weapons.push_back(new Weapon(row, col, SHELL, RIGHT, false));
            shells -= 4;
        }
        if (missiles > 0) {
            weapons.push_back(new Weapon(row, col, MISSILE, NONE, false));
            missiles--;
        }
    }
}
