#include "Item.h"

Item::Item(int r, int c, ItemType t) : GameObject(r, c, COL_ITEM), type(t) {}

void Item::draw(Gui& gui) {
    gui.paintat(row, col, type == WEAPON_PACK ? 'W' : '+', color);
}
