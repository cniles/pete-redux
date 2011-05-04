#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <btBulletDynamicsCommon.h>

#include "animation.h"
#include "debug.h"

class GameState;
class GameObject {
 protected:
  GameState* gamestate;
  AnimationTimer animation_timer;

  bool disable_draw;

  btVector3 position;
  int direction;
 public:
  GameObject(GameState* gamestate, btVector3 position, Animation* animation);
  virtual ~GameObject();

  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) { position = new_position; }

  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }

  inline int getAnimationState() { return animation_timer.getState(); }
  inline AnimationTimer* getAnimationTimer() { return &animation_timer; }
 
  inline void disableDraw() { disable_draw = true; }

  virtual void draw();
  virtual void update(float) = 0;
};

#endif
