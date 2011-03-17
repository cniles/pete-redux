#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"

class GameObject;
class GameObjectMotionState : public btMotionState {
  inline void setObject(GameObject* object) { this->object = object; }
  ~GameObjectMotionState();
  void getWorldTransform(btTransform& world_transform) const;
  void setWorldTransform(const btTransform& world_transform);
 protected:
  GameObject* object;
  btTransform position;
 public:
  GameObjectMotionState(const btTransform& initial_position, GameObject* object);
};

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
  GameObject(float x, float y, GameState* gamestate, btCollisionShape* collision_shape = &default_collision_shape);
  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) { position = new_position; }
  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }
  virtual void draw() = 0;
  virtual void update(float) = 0;
  friend class PlatformAI;
};

class PhysicalGameObject : public GameObject {
};

#endif
