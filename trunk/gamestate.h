#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <vector>
#include "bullet.h"
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
  btDynamicsWorld* dynamics_world;
  void initialize();
};

#endif
