#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>

#include "bulletwrap.h"
#include "gameobject.h"
#include "level.h"
#include "player.h"

struct GameState {
  Level level;
  Player* player;
  std::vector<GameObject*> objects;
  std::vector<btRigidBody*> level_bodies;
  std::vector<btCollisionShape*> level_shapes;
  GameState(Level level);
  ~GameState();
  BulletWrap bullet;
  void initialize();
};

#endif
