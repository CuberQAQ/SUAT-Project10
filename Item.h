#ifndef _ITEM_H_
#define _ITEM_H_

#include "GameObject.h"

class Item : public GameObject {
public:
    ItemType type;

    Item(int r, int c, ItemType t);
    void draw(Gui& gui) override;
};

#endif
