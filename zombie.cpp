#include <iostream>

#include "zombie.h"

Animation Zombie::animation = Animation();

void Zombie::StateDead::onEnter() {}
void Zombie::StateDead::onUpdate(float) {}
void Zombie::StateDead::onLeave() {}




void Zombie::StateDying::onEnter() {}
void Zombie::StateDying::onUpdate(float dt) {
  if(owner->getAnimationState() == AnimationTimer::STOPPED) {
    owner->changeState(new Zombie::StateDead(owner));
  }
}
void Zombie::StateDying::onLeave() {}





void Zombie::StateMoving::onEnter() {
  btVector3 destination = owner->getFurthestFacingPointOnPlatform();
  AI_DEBUG_OUT("Entered ZombieMoveState");
  if((owner->getPosition() - destination).fuzzyZero()) {
    owner->changeDirection();
    destination = owner->getFurthestFacingPointOnPlatform();    
    if((owner->getPosition() - destination).fuzzyZero()) {
      owner->changeDirection();
      owner->changeState(new Zombie::StateIdle(owner));
    }
  }
  owner->setDestination(destination);    
}
void Zombie::StateMoving::onUpdate(float dt) {
  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 destination = owner->getDestination();
  btVector3 current_position = owner->getPosition();
  if(current_position.distance(destination) <= in_range_eps) {
    owner->changeDirection();
    owner->setDestination(owner->getFurthestFacingPointOnPlatform());
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
    }
  }
  // Nudge the object in the correct direction.
  btVector3 force(owner->getDirection(), 0.0f, 0.0f);
  force = force * 3;
  owner->getRigidBody()->applyCentralForce(force);
}
void Zombie::StateMoving::onLeave() {}



void Zombie::StateIdle::onEnter() {}
void Zombie::StateIdle::onUpdate(float dt) {}
void Zombie::StateIdle::onLeave() {}



Zombie::Zombie(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, default_collision_scheme, new Zombie::StateMoving(this)), health(INITIAL_HEALTH),
    can_attack(true) {
}

void Zombie::takeDamage(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      CollisionScheme scheme = default_collision_scheme;
      scheme.collision_flags = COL_LEVEL;
      changeCollisionScheme(scheme);
      animation_timer.playClipOnce(2, -1);
      changeState(new Zombie::StateDying(this));
    }
  }
}

void Zombie::loadStaticAssets() {
  animation = Animation("gfx/zombie");
}
