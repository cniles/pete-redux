#ifndef ENEMY_H_
#define ENEMY_H_

#include "statemachineobject.h"

class Enemy : public StateMachineObject {
  const static int INITIAL_HEALTH = 1;
  static Animation animation;

  //btVector3 destination;
  int health;

  MAKE_STATE(StateDead, Enemy);
  MAKE_STATE(StateMove, Enemy);
  MAKE_STATE(StateIdle, Enemy);
 public:
  Enemy(GameState*, btVector3);
  //void notifyWasShot(int, int);
  static void loadStaticAssets();
};

#endif
