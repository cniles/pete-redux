#ifndef BULLETWRAP_H_
#define BULLETWRAP_H_

#include <btBulletDynamicsCommon.h>

struct BulletWrap {
  BulletWrap();
  ~BulletWrap();
  btBroadphaseInterface* broadphase;
  btDefaultCollisionConfiguration* collision_configuration;
  btCollisionDispatcher* dispatcher;
  btSequentialImpulseConstraintSolver* solver;
  btDiscreteDynamicsWorld* dynamics_world;
};

#endif
