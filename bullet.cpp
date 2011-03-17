#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h>
#include <BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h>
#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include <BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h>
#include <btBulletDynamicsCommon.h>

btDynamicsWorld* createPhysicsWorld() {
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();
  btCollisionConfiguration* collision_configuration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collision_configuration);
 
  btVoronoiSimplexSolver* simplex = new btVoronoiSimplexSolver();
  btMinkowskiPenetrationDepthSolver* pd_solver = new btMinkowskiPenetrationDepthSolver();

  btConvex2dConvex2dAlgorithm::CreateFunc* convex_algorithm_2d = new btConvex2dConvex2dAlgorithm::CreateFunc(simplex, pd_solver);

  dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d);
  dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d); 
  dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, CONVEX_2D_SHAPE_PROXYTYPE, convex_algorithm_2d);
  dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE, BOX_2D_SHAPE_PROXYTYPE, new btBox2dBox2dCollisionAlgorithm::CreateFunc());

  btConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
  btDynamicsWorld* dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
  dynamics_world->setGravity(btVector3(0,-10,0));
  return dynamics_world;
}
