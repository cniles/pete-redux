#ifndef ZOMBIE_H_
#define ZOMBIE_H_
#define ZOMBIE_INITIAL_HEALTH_ 3

#include "statemachineobject.h"

MAKE_STATE(ZombieStateMoving)
MAKE_STATE(ZombieStateIdle)
MAKE_STATE(ZombieStateDying)
MAKE_STATE(ZombieStateDead)

class Zombie : public StateMachineObject {
  const static int INITIAL_HEALTH = ZOMBIE_INITIAL_HEALTH_;
  static Animation animation;
  int health;
 public:
  Zombie(GameState*, btVector3 position);
  void takeDamage(int, int);
  static void loadStaticAssets();
};

#endif
