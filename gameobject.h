#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "bulletwrap.h"

class GameObject {
 protected:
  btVector3 position;
  int direction;
 public:
  GameObject(float x, float y);
  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) {position = new_position;}
  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }
  virtual void draw() = 0;
  virtual void update(float) = 0;
  friend class PlatformAI;
};

#endif
