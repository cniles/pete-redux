#ifndef SOLDIER_H_
#define SOLDIER_H_

#include "statemachineobject.h"

class Soldier : public StateMachineObject {
  const static int INITIAL_HEALTH = 3;
  const static int ZOMBIE_DAMAGE = 5;
  const static float WALK_SPEED;
  const static float ATTACK_COOLDOWN;
  const static float MOVE_EPS;
  const static float CHASE_COOLDOWN;

  static Animation animation;

  int health;
  int timer;
  btVector3 destination;
  bool can_attack;
  float attack_timer;

  MAKE_STATE(StateMoving, Soldier);
  MAKE_STATE(StateIdle, Soldier);
  MAKE_STATE(StateDying, Soldier);
  MAKE_STATE(StateDead, Soldier);
  MAKE_STATE(StateChase, Soldier);

 public:
  Soldier(GameState*, const btVector3& position);
  void notifyWasShot(int, int);
  static void loadStaticAssets();
  btVector3 getChaseDestination();
};

#endif
