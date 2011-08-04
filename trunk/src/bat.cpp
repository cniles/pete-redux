#include "bat.h"
#include "physicsobject.h"
#include "gamestate.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "dijkstra.h"

const float Bat::MASS = 1.0f;
const float Bat::ATTACK_COOLDOWN = 0.5f;
const float Bat::GRAVITY = 0.0f;
const float Bat::DETECT_RANGE = 9.0f;

void Bat::StateMove::onEnter() {
  owner->timer = 0;
}
void Bat::StateMove::onUpdate(float dt) {
  if(owner->getDistance2ToPlayer() < Bat::DETECT_RANGE) {
    owner->changeState(new Bat::StateAttack(owner));
    return;
  }
  if((owner->destination - owner->position).length() < 0.1f || owner->timer <= 0) {
    owner->destination = owner->getRandomMove();
    btVector3 direction = owner->destination - owner->position;

    direction.normalize();
    owner->rigid_body->setLinearVelocity(direction);
    owner->timer = Bat::MOVEMENT_TIMEOUT;
  }
  owner->timer -= dt;
}
void Bat::StateMove::onLeave() {}

void Bat::StateDead::onEnter() {
  owner->animation_timer.playClipOnce(2, -1);
  owner->rigid_body->setGravity(btVector3(0.0f, -8.0f, 1.0f));
}
void Bat::StateDead::onUpdate(float dt) {
  if(owner->rigid_body->getLinearVelocity().fuzzyZero()) {
    owner->removeCollisionBody();
    owner->stopUpdate();
  }
}
void Bat::StateDead::onLeave() {}

int costFunc(PathNode* a, PathNode* b) {
  return 1;
}

void Bat::StateAttack::onEnter() {
  owner->timer = 0;
  int graph_size = owner->gamestate->pathgraph->getGraphSize();
  PathNode* graph  = owner->gamestate->pathgraph->getGraph();
  PathNode* graph_end = graph+graph_size;
  int x_pos = owner->position.getX();
  int y_pos = owner->position.getY();
  int source = (&*std::find(graph, graph_end, PathNode(x_pos, y_pos))) - graph;
  dijkstra_search<PathNode>(graph, graph_size, source, costFunc);
  PathNode* player = (&*std::find(graph, graph_end, PathNode(owner->gamestate->player->getX(), owner->gamestate->player->getY())));
  if(player != graph_end) {
    while(player != NULL) {
      owner->path.push(player);
      player = player->dij_prev;
    }
  }
}
void Bat::StateAttack::onUpdate(float dt) {
  
  std::cerr << "Fail 1" << std::endl;
  btVector3 path_position(owner->path.top()->x_pos + 0.5f, owner->path.top()->y_pos + 0.5f, 0.0f);
  std::cerr << "Fail 2" << std::endl;
  if((path_position - owner->position).length() < 0.1f) {
    owner->path.pop();
    std::cerr << "Fail 3" << std::endl;
    std::cerr << "Fail 5" << std::endl;
    if(owner->path.empty() || owner->path.top() == NULL) {
      owner->changeState(new Bat::StateMove(owner));
      return;
    }
  }
  std::cerr << "Fail 4" << std::endl;
  btVector3 velocity = path_position - owner->position;
  velocity.normalize();
  owner->rigid_body->setLinearVelocity(velocity);
}
void Bat::StateAttack::onLeave() {}

//Bat::State::onLeave() {}
//Bat::State::onUpdate() {}
//Bat::State::onLeave() {}

btBox2dShape Bat::collision_shape(btVector3(0.2f, 0.2f, 0.1f));
CollisionScheme Bat::collision_scheme(COL_ENEMY, COL_LEVEL | COL_PLAYER, Bat::MASS, &Bat::collision_shape);
Animation Bat::animation;

Bat::Bat(GameState* gamestate, const btVector3& position)
  : StateMachineObject(gamestate, position, &animation, Bat::collision_scheme, new Bat::StateMove(this)),
    health(Bat::INITIAL_HEALTH) {
  rigid_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

struct int_tuple {
  int_tuple() : x(0), y(0) {}
  int_tuple(int x, int y) : x(x), y(y) {}
  int x, y;
};

btVector3 Bat::getRandomMove() {
  int x = position.getX();
  int y = position.getY();
  std::vector<int_tuple> valid_moves;
  for(int i = -1; i < 2; i++) {
    for(int j = -1; j < 2; j++) {
      if(i==0 && j==0) continue;
      if(abs(i)==abs(j)) continue;
      if(gamestate->level.getTile(x+i, y+j) == 0) {
	valid_moves.push_back(int_tuple(i, j));
      }
    }
  }
  btVector3 result(x+0.5f, y+0.5f, 0.0f);
  if(valid_moves.size()) {
    int move = (rand() % valid_moves.size());
    btVector3 change(valid_moves[move].x, valid_moves[move].y, 0);
    result += change;
  }  
  return result;
}

void Bat::notifyWasShot(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      changeState(new Bat::StateDead(this));
    }
  }
}

void Bat::loadStaticAssets() {
  animation = Animation("gfx/bat");
}
