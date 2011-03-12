#include "bulletwrap.h"

BulletWrap::~BulletWrap() {
  delete dynamics_world;
  delete solver;
  delete dispatcher;
  delete collision_configuration;
  delete broadphase;
}

BulletWrap::BulletWrap() {
  broadphase = new btDbvtBroadphase();
  collision_configuration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collision_configuration);
  solver = new btSequentialImpulseConstraintSolver;
  dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
  dynamics_world->setGravity(btVector3(0,-10,0));
}
