#ifndef ZOMBIE_H_
#define ZOMBIE_H_
#define ZOMBIE_INITIAL_HEALTH_ 3
#include "animation.h"
#include "gamestate.h"
#include "gameobject.h"
#include "platformai.h"

MAKE_STATE(ZombieStateMoving)
MAKE_STATE(ZombieStateIdle)
MAKE_STATE(ZombieStateDying)
MAKE_STATE(ZombieStateDead)

class Zombie : public GameObject {
  const static int INITIAL_HEALTH = ZOMBIE_INITIAL_HEALTH_;
  PlatformAI ai;
  int health;
 public:
  static Animation animation;
  Zombie(float x, float y, GameState*);
  void update(float dt);
  void draw();
  void takeDamage(int, int);
};

#endif
