#include "Destroyer.h"
#include <cstdlib>
#include <cmath>

Destroyer::Destroyer(int r, int c) : EnemyShip(r, c, DESTROYER, 10, 8) { // 10 HP, RED (8)
    shells = 10;
    torpedoes = 2;
    shootInterval = 8;
}

void Destroyer::draw(Gui& gui) {
    gui.paintat(row, col, 'O', color);
}

void Destroyer::update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) {
    // Movement (Same as Gunboat but could be different)
    moveTimer++;
    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        Direction d = NONE;
        bool useAI = (rand() % 100 < 90) && player;
        
        if (useAI) {
            int dRow = player->row - row;
            int dCol = player->col - col;
            if (abs(dRow) > abs(dCol)) d = (dRow > 0) ? DOWN : UP;
            else d = (dCol > 0) ? RIGHT : LEFT;
        } else {
            d = (Direction)(rand() % 4);
        }

        int nextR = row, nextC = col;
        switch(d) {
            case UP: nextR--; break;
            case DOWN: nextR++; break;
            case LEFT: nextC--; break;
            case RIGHT: nextC++; break;
            default: break;
        }

        bool safe = (nextR >= 0 && nextR < maxR && nextC >= 0 && nextC < maxC);
        
        if (safe) {
            for (auto obs : obstacles) {
                if (obs->row == nextR && obs->col == nextC) { safe = false; break; }
            }
            if (safe && player) {
                if (abs(player->row - nextR) <= 1 && abs(player->col - nextC) <= 1) safe = false;
            }
            if (safe) {
                for (auto other : enemies) {
                    if (other == this || !other->active) continue;
                    if (other->type == CRUISER) {
                        if (other->row == nextR && (nextC >= other->col - 1 && nextC <= other->col + 1)) safe = false;
                    } else {
                        if (other->row == nextR && other->col == nextC) safe = false;
                    }
                    if (!safe) break;
                }
            }
        }

        if (safe) {
            dir = d;
            row = nextR;
            col = nextC;
        } else if (useAI && player) {
            int dRow = player->row - row;
            int dCol = player->col - col;
            if (abs(dRow) > abs(dCol)) dir = (dRow > 0) ? DOWN : UP;
            else dir = (dCol > 0) ? RIGHT : LEFT;
        }
    }

    // Shooting
    shootTimer++;
    if (shootTimer >= shootInterval) {
        shootTimer = 0;
        bool aligned = false;
        if (player) {
            if (dir == UP || dir == DOWN) {
                if (abs(col - player->col) <= 4) aligned = true;
            } else {
                if (abs(row - player->row) <= 2) aligned = true;
            }
            if (aligned) {
                if (dir == UP && player->row > row) aligned = false;
                if (dir == DOWN && player->row < row) aligned = false;
                if (dir == LEFT && player->col > col) aligned = false;
                if (dir == RIGHT && player->col < col) aligned = false;
            }
        } else {
            aligned = (rand() % 5 == 0);
        }

        if (aligned) {
            if (shells > 0) {
                weapons.push_back(new Weapon(row, col, SHELL, dir, false));
                shells--;
            }
            if (torpedoes > 0) {
                Direction leftDir, rightDir;
                switch(dir) {
                    case UP: leftDir = LEFT; rightDir = RIGHT; break;
                    case DOWN: leftDir = RIGHT; rightDir = LEFT; break;
                    case LEFT: leftDir = DOWN; rightDir = UP; break;
                    case RIGHT: leftDir = UP; rightDir = DOWN; break;
                    default: leftDir = LEFT; rightDir = RIGHT; break;
                }
                weapons.push_back(new Weapon(row, col, TORPEDO, leftDir, false));
                weapons.push_back(new Weapon(row, col, TORPEDO, rightDir, false));
                torpedoes -= 2;
            }
        }
    }
}
