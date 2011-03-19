#ifndef PHYSICSOBJECT_H_
#define PHYSICSOBJECT_H_

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>

#include "gameobject.h"
#include "bullet.h"

struct CollisionScheme {
  CollisionScheme(int group, int flags, btScalar mass, btCollisionShape* shape);
  btCollisionShape* collision_shape;
  btScalar mass;
  int collision_group;
  int collision_flags;
};


class PhysicsObject : public GameObject {
 private:
  bool collision_with_player_detected;

 protected:
  static btCapsuleShape child_default_collision_shape;
  static btConvex2dShape default_collision_shape;
  static CollisionScheme default_collision_scheme;

  PhysicsObject(GameState* gamestate, 
	        btVector3 position,
		Animation* animation,
		CollisionScheme scheme);
  ~PhysicsObject();

  btRigidBody* rigid_body;
  void changeCollisionScheme(CollisionScheme scheme);
  void removeCollisionBody();

 public:
  inline btRigidBody* getRigidBody() { return rigid_body; }
  inline bool notifyCollisionWithPlayer() { collision_with_player_detected = true; }
  bool collisionWithPlayerDetected();
};

class GameObjectMotionState : public btMotionState {
  ~GameObjectMotionState();
  void getWorldTransform(btTransform& world_transform) const;
  void setWorldTransform(const btTransform& world_transform);
 protected:
  GameObject* object;
  btTransform position;
 public:
  GameObjectMotionState(const btTransform& initial_position, GameObject* object);
};

#endif
