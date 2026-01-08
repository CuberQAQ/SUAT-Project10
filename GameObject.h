#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Gui.h"
#include "Types.h"

class GameObject {
public:
    int row, col;
    int color;
    bool active;
    
    GameObject(int r, int c, int clr);
    virtual ~GameObject();
    virtual void draw(Gui& gui) = 0;
    virtual void update();
};

#endif
