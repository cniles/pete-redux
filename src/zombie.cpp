#include <iostream>

#include "zombie.h"
#include "gamestate.h"

Animation Zombie::animation = Animation();

Zombie::Zombie(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, default_collision_scheme, new Zombie::StateMoving(this)), health(INITIAL_HEALTH),
    can_attack(true) {
}

void Zombie::StateDead::onEnter() {}
void Zombie::StateDead::onUpdate(float) {}
void Zombie::StateDead::onLeave() {}

void Zombie::StateDying::onEnter() {
      CollisionScheme scheme = default_collision_scheme;
      scheme.collision_flags = COL_LEVEL;
      owner->changeCollisionScheme(scheme);
      owner->animation_timer.playClipOnce(2, -1);
}
void Zombie::StateDying::onUpdate(float dt) {
  if(owner->getAnimationState() == AnimationTimer::STOPPED) {
    owner->changeState(new Zombie::StateDead(owner));
  }
}
void Zombie::StateDying::onLeave() {}

void Zombie::StateMoving::onEnter() {
  owner->destination = owner->getFurthestFacingPointOnPlatform();
  AI_DEBUG_OUT("Entered ZombieMoveState");
  if((owner->getPosition() - owner->destination).fuzzyZero()) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();    
    if((owner->getPosition() - owner->destination).fuzzyZero()) {
      owner->changeDirection();
      owner->changeState(new Zombie::StateIdle(owner));
    }
  }
}
void Zombie::StateMoving::onUpdate(float dt) {
  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 current_position = owner->getPosition();
  if(current_position.distance(owner->destination) <= in_range_eps) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();
  }
  if(owner->attack_timer > 0.0) {
    owner->attack_timer -= dt;
    if(owner->attack_timer <= 0) {
      owner->can_attack=true;
    }
  }
  if(owner->can_attack) {
    if(owner->collisionWithPlayerDetected()) {
      owner->can_attack = false;
      owner->getAnimationTimer()->playClipOnce(1,0);
      owner->attack_timer = Zombie::ATTACK_COOLDOWN;
      owner->gamestate->player->takeDamage(1,0);
    }
  }
  /*  if(owner->getDistance2ToPlayer() <= 9.0f) {
    owner->changeState(new Zombie::StateChase(owner));
    }*/
  // Nudge the object in the correct direction.
  btVector3 force(owner->getDirection(), 0.0f, 0.0f);
  force = force * 3;
  owner->getRigidBody()->applyCentralForce(force);
}
void Zombie::StateMoving::onLeave() {}

void Zombie::StateIdle::onEnter() {}
void Zombie::StateIdle::onUpdate(float dt) {}
void Zombie::StateIdle::onLeave() {}

void Zombie::StateChase::onEnter() {
  owner->timer = 0;
}
void Zombie::StateChase::onUpdate(float dt) {
  if(owner->timer <= 0 || (owner->destination - owner->position).length() < 0.1f) {
    owner->destination = owner->getChaseDestination();
    owner->timer = CHASE_COOLDOWN;
  }
  btVector3 force = owner->destination - owner->position;
  force.normalize();
  force *= 4;
  owner->getRigidBody()->applyCentralForce(force);
  owner->timer -= dt;
}
void Zombie::StateChase::onLeave() {}

void Zombie::notifyWasShot(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      changeState(new Zombie::StateDying(this));
    }
  }
}

void Zombie::loadStaticAssets() {
  animation = Animation("gfx/zombie");
}

bool isValidTile(int x, int y, Level& level) {
  if(level.getTile(x,y)==0 && level.getTile(x, y-1)==0) {
    return true; 
  }
  return false;
}

btVector3 Zombie::getChaseDestination() {
  int x = position.getX();
  int y = position.getY();
  
  if(isValidTile(x+direction, y, gamestate->level)) {
    x += direction;
  }
  else if(isValidTile(x+direction, y-1, gamestate->level)) {
    x += direction;
    y -= 1;
  }
  else if(isValidTile(x+direction, y+1, gamestate->level)) {
    x += direction;
    y += 1;
  }
  return btVector3(x, y, 0.0f);
}
