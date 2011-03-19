#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "debug.h"

#include <btBulletDynamicsCommon.h>

#include "damagetaker.h"
#include "animation.h"

class GameState;
class GameObject : public DamageTaker {
 protected:
  GameState* gamestate;
  AnimationTimer animation_timer;

  btVector3 position;
  int direction;
 public:
  GameObject(GameState* gamestate, btVector3 position, Animation* animation);
  ~GameObject();

  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) { position = new_position; }

  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }

  inline int getAnimationState() { return animation_timer.getState(); }
  inline AnimationTimer* getAnimationTimer() { return &animation_timer; }
 
  virtual void draw();
  virtual void update(float) = 0;
  virtual void takeDamage(int damage, int type);
};

#endif
