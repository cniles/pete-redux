#ifndef BULLET_H_
#define BULLET_H_

#include <btBulletDynamicsCommon.h>

#define BIT(x) (1<<(x))
enum CollisionType {
  COL_NOTHING = 0, 
  COL_PLAYER = BIT(0),
  COL_LEVEL = BIT(1),
  COL_ENEMY = BIT(2)
};

btDynamicsWorld* createPhysicsWorld();

#endif
