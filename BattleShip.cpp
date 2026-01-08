#include "BattleShip.h"

BattleShip::BattleShip(int r, int c) : Ship(r, c, 1000, COL_PLAYER), coins(0), shells(100), missiles(5), shellsFired(0), missilesFired(0), shellRegenTimer(0), missileRegenTimer(0) {
    moveInterval = 0; // Player moves instantly on key press
}

void BattleShip::update() {
    // Shell regeneration (1 per 10 updates)
    shellRegenTimer++;
    if (shellRegenTimer >= 10) {
        if (shells < 100) shells++;
        shellRegenTimer = 0;
    }
    
    // Missile regeneration (1 per 1000 updates)
    missileRegenTimer++;
    if (missileRegenTimer >= 1000) {
        if (missiles < 5) missiles++;
        missileRegenTimer = 0;
    }
}

void BattleShip::draw(Gui& gui) {
    if (dir == LEFT || dir == RIGHT) {
        gui.paintat(row, col-1, '<', color);
        gui.paintat(row, col, '=', color);
        gui.paintat(row, col+1, '>', color);
    } else {
        gui.paintat(row-1, col, '^', color);
        gui.paintat(row, col, '"', color); // Using " for center vertical to match '' requirement
        gui.paintat(row+1, col, 'v', color); // Using v for bottom
    }
}

void BattleShip::move(Direction d, int maxR, int maxC, const std::vector<Obstacle*>& obstacles) {
    dir = d;
    int nextR = row;
    int nextC = col;
    
    switch(d) {
        case UP: nextR--; break;
        case DOWN: nextR++; break;
        case LEFT: nextC--; break;
        case RIGHT: nextC++; break;
        default: break;
    }

    // Boundary checks (accounting for ship size)
    if (dir == LEFT || dir == RIGHT) {
        if (nextC < 1 || nextC >= maxC - 1 || nextR < 0 || nextR >= maxR) return;
    } else {
        if (nextR < 1 || nextR >= maxR - 1 || nextC < 0 || nextC >= maxC) return;
    }

    // Obstacle collision
    for (auto obs : obstacles) {
        // Check collision with the ship body
        // Horizontal: (r, c-1), (r, c), (r, c+1)
        // Vertical: (r-1, c), (r, c), (r+1, c)
        bool collide = false;
        if (dir == LEFT || dir == RIGHT) {
            if (obs->row == nextR && (obs->col == nextC-1 || obs->col == nextC || obs->col == nextC+1)) collide = true;
        } else {
            if (obs->col == nextC && (obs->row == nextR-1 || obs->row == nextR || obs->row == nextR+1)) collide = true;
        }
        if (collide) return;
    }

    row = nextR;
    col = nextC;
}

void BattleShip::shoot(std::vector<Weapon*>& weapons, bool triple, Direction forceDir) {
    if (shells <= 0) return;
    
    Direction d = (forceDir != NONE) ? forceDir : dir;

    if (triple) {
        if (shells < 3) return;
        shells -= 3;
        
        // Center shot
        weapons.push_back(new Weapon(row, col, SHELL, d, true));
        
        // Side shots (parallel volley)
        if (d == LEFT || d == RIGHT) {
            weapons.push_back(new Weapon(row - 1, col, SHELL, d, true));
            weapons.push_back(new Weapon(row + 1, col, SHELL, d, true));
        } else {
            // UP or DOWN
            weapons.push_back(new Weapon(row, col - 1, SHELL, d, true));
            weapons.push_back(new Weapon(row, col + 1, SHELL, d, true));
        }
        
        shellsFired += 3;
    } else {
        shells--;
        weapons.push_back(new Weapon(row, col, SHELL, d, true));
        shellsFired++;
    }
}

void BattleShip::launchMissile(std::vector<Weapon*>& weapons, GameObject* target) {
    if (missiles <= 0) return;
    missiles--;
    // Missile direction initially forward
    Weapon* m = new Weapon(row, col, MISSILE, dir, true);
    // Logic to follow target needs to be handled in update loop or Weapon needs target ptr
    weapons.push_back(m);
    missilesFired++;
}
