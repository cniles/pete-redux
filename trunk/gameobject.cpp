#include "gameobject.h"

GameObject::GameObject(float x, float y) {
  position = btVector3(x, y, 0);
  direction = -1;
}
