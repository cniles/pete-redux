#include "darkchampion.h"
#include "gamestate.h"

Animation DarkChampion::animation;

void DarkChampion::StateIdle::onEnter() {}
void DarkChampion::StateIdle::onUpdate(float dt) {}
void DarkChampion::StateIdle::onLeave() {}




void DarkChampion::StateMoving::onEnter() {
  owner->destination = owner->getFurthestFacingPointOnPlatform();
  AI_DEBUG_OUT("Entered DarkChampionMoveState");
  if((owner->getPosition() - owner->destination).fuzzyZero()) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();    
    if((owner->getPosition() - owner->destination).fuzzyZero()) {
      owner->changeDirection();
      owner->changeState(new DarkChampion::StateIdle(owner));
    }
  }
}

void DarkChampion::StateMoving::onUpdate(float dt) {
  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 current_position = owner->getPosition();
  if(current_position.distance(owner->destination) <= in_range_eps) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();
  }
  if(owner->lookForPlayer(btVector3(owner->getDirection() * VIEW_RANGE, 0, 0))) {
    owner->changeState(new DarkChampion::StateAttack(owner));
  }
  btVector3 force(owner->getDirection(), 0.0f, 0.0f);
  force = force * 3;
  owner->getRigidBody()->applyCentralForce(force);
}
void DarkChampion::StateMoving::onLeave() {}





void DarkChampion::StateAttack::onEnter() {
  AI_DEBUG_OUT("Entered DarkChampion::StateAttack"); 
}
void DarkChampion::StateAttack::onUpdate(float dt) {}
void DarkChampion::StateAttack::onLeave() {}





void DarkChampion::StateDying::onEnter() {
      CollisionScheme scheme = default_collision_scheme;
      scheme.collision_flags = COL_LEVEL;
      owner->changeCollisionScheme(scheme);
      owner->animation_timer.playClipOnce(2, -1);
}
void DarkChampion::StateDying::onUpdate(float dt) {
  if(owner->getAnimationState() == AnimationTimer::STOPPED) {
    owner->changeState(new DarkChampion::StateDead(owner));
  }
}
void DarkChampion::StateDying::onLeave() {}

void DarkChampion::StateDead::onEnter() {}
void DarkChampion::StateDead::onUpdate(float dt) {}
void DarkChampion::StateDead::onLeave() {}

DarkChampion::DarkChampion(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme, new DarkChampion::StateMoving(this)), health(INITIAL_HEALTH), can_attack(true) {
  animation_timer.setClip(0);
}

void DarkChampion::loadStaticAssets() {
  animation = Animation("gfx/darkchampion");
}

void DarkChampion::notifyWasShot(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      changeState(new DarkChampion::StateDying(this));
    }
  }
}
