#ifndef PLAYER_H
#define PLAYER_H

#include "keystates.h"
#include "animation.h"

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"

#define PLAYER_INITIAL_HEALTH 99
#define PLAYER_INITIAL_AMMO 12

#define PLAYER_COLLISION_WIDTH 0.25f
#define PLAYER_COLLISION_HEIGHT 0.4f

#define PLAYER_ACCELERATION 10
#define PLAYER_IN_AIR_ACCELERATION 3

#define PLAYER_JUMP_VELOCITY 4.8f
#define PLAYER_JUMP_MAX_TIME 0.25f
#define PLAYER_JUMP_MIN_TIME 0.025f

#define PLAYER_SHOOT_PAUSE_TIME 0.75f
#define PLAYER_SHOTGUN_FORCE 150.0f

#define PLAYER_ON_GROUND_TOLERANCE 0.2


class GameState;
class Player {
  btRigidBody* rigid_body;
  btTransform position;
  GameState* gamestate;

  static Animation animation;
  AnimationTimer animation_timer;
  float shoot_timer;
  float jump_timer;
  int direction;
  int health;
  int ammo;
  
  bool jumping;
  bool on_ground;
  bool hit_head;
  bool can_shoot;
  bool ctrl_released;
  bool flip_sprite;

  void handleKeyStates(const KeyStates&);
  void calcIsOnGround();
  void calcHitHead();
  void fireShotgun();
  void applyJumpVelocity();
 public:
  static btCapsuleShape child_collision_shape;
  static btConvex2dShape collision_shape;

  Player(int x, int y, GameState*);
  ~Player();
  bool isOnGround();
  bool hasHitHead();
  float getX();
  float getY();
  void draw() const;
  void increaseAmmo(int);
  void takeDamage(int,int);
  void increaseHealth(int);
  void sync(const Player* player);
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

