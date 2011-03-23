#ifndef DARKCHAMPION_H_
#define DARKCHAMPION_H_

#include "statemachineobject.h"

class DarkChampion : public StateMachineObject {
  const static int INITIAL_HEALTH = 1;
  static Animation animation;

  //btVector3 destination;
  int health;

  MAKE_STATE(StateDead, DarkChampion);
  MAKE_STATE(StateMove, DarkChampion);
  MAKE_STATE(StateIdle, DarkChampion);
 public:
  DarkChampion(GameState*, btVector3);
  //void notifyWasShot(int, int);
  static void loadStaticAssets();
};

#endif
