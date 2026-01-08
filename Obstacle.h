#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "GameObject.h"

class Obstacle : public GameObject {
public:
    Obstacle(int r, int c);
    void draw(Gui& gui) override;
};

#endif
