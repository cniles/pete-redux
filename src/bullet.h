#ifndef BULLET_H_
#define BULLET_H_

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>
#include <btBulletDynamicsCommon.h>

#define BIT(x) (1<<(x))
enum CollisionType {
  COL_NOTHING = 0, 
  COL_PLAYER = BIT(0),
  COL_LEVEL = BIT(1),
  COL_ENEMY = BIT(2)
};

struct BulletCore {
  BulletCore();
  ~BulletCore();
  btBroadphaseInterface* broadphase;
  btCollisionConfiguration* collision_configuration;
  btCollisionDispatcher* dispatcher;
  btVoronoiSimplexSolver* simplex;
  btMinkowskiPenetrationDepthSolver* pd_solver;
  btConstraintSolver* solver;
  btDynamicsWorld* dynamics_world;
};

BulletCore* createPhysicsWorld();
#endif
