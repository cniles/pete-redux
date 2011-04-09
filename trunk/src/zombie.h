#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include "statemachineobject.h"

class Zombie : public StateMachineObject {
  const static int INITIAL_HEALTH = 3;
  const static float ATTACK_COOLDOWN = 0.5f;
  const static float MOVE_EPS = 0.1f;
  const static float CHASE_COOLDOWN = 0.25f;

  static Animation animation;

  int health;
  int timer;
  btVector3 destination;
  bool can_attack;
  float attack_timer;

  MAKE_STATE(StateMoving, Zombie);
  MAKE_STATE(StateIdle, Zombie);
  MAKE_STATE(StateDying, Zombie);
  MAKE_STATE(StateDead, Zombie);
  MAKE_STATE(StateChase, Zombie);

 public:
  Zombie(GameState*, btVector3 position);
  void notifyWasShot(int, int);
  static void loadStaticAssets();
  btVector3 getChaseDestination();
};

#endif
