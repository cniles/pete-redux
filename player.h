#ifndef PLAYER_H
#define PLAYER_H

#include "keystates.h"
#include "animation.h"

#include <BulletCollision/CollisionShapes/btConvex2dShape.h>
#include "bullet.h"

#define PLAYER_TOP_X_SPEED 5.0f
#define PLAYER_X_ACCELERATION 7.5f
#define PLAYER_X_DECELERATION 12.0f

class GameState;
class Player {
  btRigidBody* rigid_body;

  GameState* gamestate;

  static Animation animation;
  AnimationTimer animation_timer;
  
  bool space_held;
  bool on_ground;
  
  bool flip_sprite;

  void handleKeyStates(const KeyStates&);
  void calcIsOnGround();
 public:
  static btCapsuleShape child_collision_shape;
  static btConvex2dShape collision_shape;

  Player(int x, int y, GameState*);
  ~Player();
  bool isOnGround();
  float getX();
  float getY();
  void draw() const;
  void update(Uint32 dt, const KeyStates& key_states);
  void operator=(const Player& other);
};

#endif
