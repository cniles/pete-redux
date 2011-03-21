#include "bullet.h"

BulletCore* createPhysicsWorld() {
  BulletCore* bullet_core = new BulletCore();
  
  btConvex2dConvex2dAlgorithm::CreateFunc* convex_algorithm_2d = new btConvex2dConvex2dAlgorithm::CreateFunc(bullet_core->simplex, bullet_core->pd_solver);
  bullet_core->dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d);
  bullet_core->dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d); 
  bullet_core->dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d);
  bullet_core->dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());
  bullet_core->dynamics_world->setGravity(btVector3(0,-10,0));

  return bullet_core;
}

BulletCore::BulletCore() {
  broadphase = new btDbvtBroadphase();
  collision_configuration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collision_configuration);
  simplex = new btVoronoiSimplexSolver();
  pd_solver = new btMinkowskiPenetrationDepthSolver();
  solver = new btSequentialImpulseConstraintSolver;
  dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
}

BulletCore::~BulletCore() {
  delete dynamics_world;
  delete solver;
  delete pd_solver;
  delete simplex;
  delete dispatcher;
  delete collision_configuration;
  delete broadphase;
}
