#include <iostream>

#include "zombie.h"

Animation Zombie::animation = Animation();

void ZombieStateDead::onEnter() {}
void ZombieStateDead::onUpdate(float) {}
void ZombieStateDead::onLeave() {}

void ZombieStateDying::onEnter() {}
void ZombieStateDying::onUpdate(float dt) {
  if(owner->getAnimationState() == AnimationTimer::STOPPED) {
    owner->changeState(new ZombieStateDead(owner));
  }
}
void ZombieStateDying::onLeave() {}

void ZombieStateMoving::onEnter() {
  btVector3 destination = owner->getFurthestFacingPointOnPlatform();
  
  if((owner->getPosition() - destination).fuzzyZero()) {
    owner->changeDirection();
    destination = owner->getFurthestFacingPointOnPlatform();
    
    if((owner->getPosition() - destination).fuzzyZero()) {
      owner->changeDirection();
      owner->changeState(new ZombieStateIdle(owner));
    }
  }
  owner->setDestination(destination);    
}
void ZombieStateMoving::onLeave() {}
void ZombieStateMoving::onUpdate(float dt) {
  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 destination = owner->getDestination();
  btVector3 current_position = owner->getPosition();
  if(current_position.distance(destination) <= in_range_eps) {
    owner->changeDirection();
    owner->setDestination(owner->getFurthestFacingPointOnPlatform());
  }

  // Nudge the object in the correct direction.
  btVector3 force(owner->getDirection(), 0.0f, 0.0f);
  force = force * 3;
  owner->getRigidBody()->applyCentralForce(force);
}

void ZombieStateIdle::onEnter() {}
void ZombieStateIdle::onUpdate(float dt) {}
void ZombieStateIdle::onLeave() {}

Zombie::Zombie(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, default_collision_scheme, new ZombieStateMoving(this)), health(INITIAL_HEALTH) {
}

void Zombie::takeDamage(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      CollisionScheme scheme = default_collision_scheme;
      scheme.collision_flags = COL_LEVEL;
      changeCollisionScheme(scheme);
      animation_timer.playClipOnce(2, -1);
      changeState(new ZombieStateDying(this));
    }
  }
}

void Zombie::loadStaticAssets() {
  animation = Animation("gfx/zombie");
}
