#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"

class GameState;
class GameObject {
 protected:
  static btCapsuleShape child_default_collision_shape;
  static btConvex2dShape default_collision_shape;
  btVector3 position;
  int direction;
  btRigidBody* rigid_body;
  GameState* gamestate;
 public:
  GameObject(float x, float y, GameState* gamestate, int collision_mask_type, int collision_flags = COL_PLAYER | COL_LEVEL, btCollisionShape* collision_shape = &default_collision_shape);
  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) { position = new_position; }
  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }
  inline btRigidBody* getRigidBody() { return rigid_body; }
  virtual void draw() = 0;
  virtual void update(float) = 0;
  friend class PlatformAI;
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
