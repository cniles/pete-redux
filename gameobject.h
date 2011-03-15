#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "bulletwrap.h"

class GameObject;
class GameObjectMotionState : public btMotionState {
  GameObjectMotionState(const btTransform& initial_position, GameObject* object);
  inline void setObject(GameObject* object) { this->object = object; }
  ~GameObjectMotionState();
  virtual void getWorldTransform(btTransform& world_transform);
  virtual void setWorldTransform(const btTransform& world_transform);
 protected:
  GameObject* object;
  btTransform position;
};

class GameObject {
 protected:
  btVector3 position;
  int direction;
  btCollisionObject* collision_object;
  btRigidBody* rigid_body;
 public:
  GameObject(float x, float y);
  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) {position = new_position;}
  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }
  virtual void draw() = 0;
  virtual void update(float) = 0;
  friend class PlatformAI;
};

#endif
