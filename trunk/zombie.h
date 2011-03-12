#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include "animation.h"
#include "gamestate.h"
#include "gameobject.h"
#include "platformai.h"

MAKE_STATE(ZombieStateMoving)
MAKE_STATE(ZombieStateIdle)

class Zombie : public GameObject {
  AnimationTimer animation_timer;
  PlatformAI ai;
 public:
  static Animation animation;
  Zombie(float x, float y, GameState*);
  void update(float dt);
  void draw();
};

#endif
