#include "bat.h"
#include "physicsobject.h"
#include "gamestate.h"
#include <stdlib.h>
#include <vector>

void Bat::StateMove::onEnter() {
  owner->timer = 0;
}
void Bat::StateMove::onUpdate(float dt) {
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

void Bat::StateAttack::onEnter() {}
void Bat::StateAttack::onUpdate(float dt) {}
void Bat::StateAttack::onLeave() {}

//Bat::State::onLeave() {}
//Bat::State::onUpdate() {}
//Bat::State::onLeave() {}

btBox2dShape Bat::collision_shape(btVector3(0.2f, 0.2f, 0.1f));
CollisionScheme Bat::collision_scheme(COL_ENEMY, COL_LEVEL | COL_PLAYER, Bat::MASS, &Bat::collision_shape);
Animation Bat::animation;

Bat::Bat(GameState* gamestate, btVector3 position)
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
