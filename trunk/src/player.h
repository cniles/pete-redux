#ifndef PLAYER_H
#define PLAYER_H

#include "keystates.h"
#include "animation.h"

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"

#define PLAYER_TOP_X_SPEED 5.0f
#define PLAYER_X_ACCELERATION 7.5f
#define PLAYER_X_DECELERATION 12.0f
#define SHOOT_PAUSE_TIME 0.75f

class GameState;
class Player {
  btRigidBody* rigid_body;
  btTransform position;
  GameState* gamestate;

  static Animation animation;
  AnimationTimer animation_timer;
  float shoot_timer;
  int direction;
  int health;
  int ammo;
  
  bool space_held;
  bool on_ground;
  bool can_shoot;
  bool ctrl_released;
  bool flip_sprite;

  void handleKeyStates(const KeyStates&);
  void calcIsOnGround();
  void fireShotgun();
 public:
  static btCapsuleShape child_collision_shape;
  static btConvex2dShape collision_shape;

  Player(int x, int y, GameState*);
  ~Player();
  bool isOnGround();
  float getX();
  float getY();
  void draw() const;
  void increaseAmmo(int);
  void takeDamage(int,int);
  void increaseHealth(int);
  int getHealth() { return health; }
  int getAmmo() { return ammo; }
  inline void setPosition(const btTransform& position) { this->position = position; }
  inline btVector3 getPosition() { return this->position.getOrigin(); }
  void update(Uint32 dt, const KeyStates& key_states);
  void operator=(const Player& other);
};

class PlayerMotionState : public btMotionState {
  ~PlayerMotionState();
  void getWorldTransform(btTransform& world_transform) const;
  void setWorldTransform(const btTransform& world_transform);
 protected:
  Player* player;
  btTransform position;
 public:
  PlayerMotionState(const btTransform& initial_position, Player* player);
};

#endif

