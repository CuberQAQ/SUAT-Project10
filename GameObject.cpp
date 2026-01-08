#include "GameObject.h"

GameObject::GameObject(int r, int c, int clr) : row(r), col(c), color(clr), active(true) {}

GameObject::~GameObject() {}

void GameObject::update() {}
