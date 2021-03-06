#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#include <vector>
#include "bullet.h"
#include "gameobject.h"
#include "level.h"
#include "pathgraph.h"
#include "player.h"
#include "debug.h"

struct GameState {
  Level level;
  PathGraph* pathgraph;
  Player* player;
  bool exit_toggled;
  std::vector<GameObject*> objects;
  std::vector<btRigidBody*> level_bodies;
  std::vector<btCollisionShape*> level_shapes;
  GameState(Level level);
  ~GameState();
  BulletCore* bullet_core;
  btDynamicsWorld* dynamics_world;
};

#endif
