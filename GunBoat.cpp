#include "GunBoat.h"
#include <cstdlib>
#include <cmath>

GunBoat::GunBoat(int r, int c) : EnemyShip(r, c, GUNBOAT, 1, 5) { // 1 HP, CYAN (5)
    shells = 10;
    shootInterval = 6;
}

void GunBoat::draw(Gui& gui) {
    gui.paintat(row, col, 'o', color);
}

void GunBoat::update(int maxR, int maxC, const std::vector<Obstacle*>& obstacles, BattleShip* player, std::vector<Weapon*>& weapons, const std::vector<EnemyShip*>& enemies) {
    // Movement
    moveTimer++;
    if (moveTimer >= moveInterval) {
        moveTimer = 0;
        Direction d = NONE;
        bool useAI = (rand() % 100 < 70) && player;
        
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
                    // Gunboat is 1x1, check overlap
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
            // Update direction even if blocked
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

        if (aligned && shells > 0) {
            weapons.push_back(new Weapon(row, col, SHELL, dir, false));
            shells--;
        }
    }
}
