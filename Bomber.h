#ifndef _BOMBER_H_
#define _BOMBER_H_

#include "GameObject.h"
#include "Weapon.h"
#include <vector>

class Bomber : public GameObject {
public:
    Direction flyDir; // LEFT or RIGHT
    int dropTimer;

    Bomber(int r, int c, Direction d);
    void draw(Gui& gui) override;
    void update(std::vector<Weapon*>& weapons);
};

#endif
