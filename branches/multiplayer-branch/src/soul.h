#ifndef SOUL_H_
#define SOUL_H_

#include "statemachineobject.h"

class Soul : public StateMachineObject {
  const static int INITIAL_HEALTH = 4;
  static Animation animation;

  //btVector3 destination;
  int health;

  MAKE_STATE(StateDead, Soul);
  MAKE_STATE(StateMove, Soul);
  MAKE_STATE(StateIdle, Soul);
 public:
  Soul(GameState*, const btVector3&);
  void notifyWasShot(int, int);
  static void loadStaticAssets();
};

#endif
