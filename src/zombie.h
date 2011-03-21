#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include "statemachineobject.h"

class Zombie : public StateMachineObject {
  const static int INITIAL_HEALTH = 3;
  const static float ATTACK_COOLDOWN = 0.5f;

  static Animation animation;

  int health;
  btVector3 destination;
  bool can_attack;
  float attack_timer;

  MAKE_STATE(StateMoving, Zombie);
  MAKE_STATE(StateIdle, Zombie);
  MAKE_STATE(StateDying, Zombie);
  MAKE_STATE(StateDead, Zombie);

 public:
  Zombie(GameState*, btVector3 position);
  void notifyWasShot(int, int);
  static void loadStaticAssets();
};

#endif
