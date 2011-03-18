#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"
#include "damagetaker.h"
#include "animation.h"

class GameState;
class GameObject : public DamageTaker {
 private:
  static btCapsuleShape child_default_collision_shape;
  static btConvex2dShape default_collision_shape;

 protected:
  GameState* gamestate;
  AnimationTimer animation_timer;
  btRigidBody* rigid_body;
  btVector3 position;
  int direction;

 public:
  GameObject(float x, float y, GameState* gamestate, int collision_mask_type, 
	     int collision_flags = COL_PLAYER | COL_LEVEL, 
	     btCollisionShape* collision_shape = &default_collision_shape);
  ~GameObject();
  inline btVector3 getPosition() {return position;}
  inline void setPosition(const btVector3& new_position) { position = new_position; }
  inline int getDirection() { return direction; }
  inline void changeDirection() { direction *= -1; }
  inline btRigidBody* getRigidBody() { return rigid_body; }
  inline int getAnimationState() { return animation_timer.getState(); }
  void changeCollisionScheme(int group, int flags, btCollisionShape* shape = &default_collision_shape);
  void removeCollisionBody();

  virtual void draw() = 0;
  virtual void update(float) = 0;
  virtual void takeDamage(int damage, int type);

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
