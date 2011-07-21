#ifndef BAT_H_
#define BAT_H_

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include "statemachineobject.h"

class Bat : public StateMachineObject {
  const static int INITIAL_HEALTH = 1;
  const static float MASS;
  const static float ATTACK_COOLDOWN;
  const static float GRAVITY;
  const static float DETECT_RANGE;
  const static int MOVEMENT_TIMEOUT = 3.0f;
  
  static CollisionScheme collision_scheme;
  static btBox2dShape collision_shape;
  static Animation animation;

  enum SubState {
    MOVING, PAUSING
  };

  btVector3 destination;
  int state_flag;
  float timer;  
  int health;
  bool can_attack;
  float attack_timer;

  btVector3 getRandomMove();

  MAKE_STATE(StateMove, Bat);
  MAKE_STATE(StateAttack, Bat);
  MAKE_STATE(StateDead, Bat);

 public:
  Bat(GameState*, const btVector3& position);
  static void loadStaticAssets();
  btVector3 getRandomDestination();
  void notifyWasShot(int,int);
};

#endif
