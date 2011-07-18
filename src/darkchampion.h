#ifndef DARKCHAMPION_H_
#define DARKCHAMPION_H_

#include "statemachineobject.h"

class DarkChampion : public StateMachineObject {
  const static int CHARGING = 0;
  const static int SWINGING = 1;
  const static int INITIAL_HEALTH = 5;
  const static int DAMAGE = 5;
  const static float VIEW_RANGE = 5.0f;
  const static float ATTACK_RANGE = 0.5f;
  const static float ATTACK_COOLDOWN = 1.0f;
  const static float STUNNED_COOLDOWN = 1.0f;
  const static float MAX_HANG_TIME = 3.0f;
  const static float MOVE_EPS = 0.1f;
  const static float CHASE_COOLDOWN = 0.25f;


  static Animation animation;

  float hang_timer;
  int substate;
  int health;
  int timer;
  btVector3 destination;
  btVector3 previous_position;
  bool can_attack;
  bool hung;
  float attack_timer;

  MAKE_STATE(StateIdle, DarkChampion);
  MAKE_STATE(StateMoving, DarkChampion);
  MAKE_STATE(StateAttack, DarkChampion);
  MAKE_STATE(StateDead, DarkChampion);
  MAKE_STATE(StateDying, DarkChampion);

 public:
  DarkChampion(GameState*, btVector3);
  void notifyWasShot(int, int);
  static void loadStaticAssets();
};

#endif
