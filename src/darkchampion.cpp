#include "darkchampion.h"
#include "gamestate.h"

const float DarkChampion::VIEW_RANGE = 5.0f;
const float DarkChampion::ATTACK_RANGE = 0.45f;
const float DarkChampion::BREAK_ATTACK_RANGE = 0.7f;
const float DarkChampion::ATTACK_COOLDOWN = 1.0f;
const float DarkChampion::STUNNED_COOLDOWN = 1.0f;
const float DarkChampion::CHARGE_SPEED = 1.2f;
const float DarkChampion::MAX_HANG_TIME = 3.0f;
const float DarkChampion::MOVE_EPS = 0.1f;
const float DarkChampion::CHASE_COOLDOWN = 0.25f;
const float DarkChampion::MOVE_SPEED = 0.8f;

Animation DarkChampion::animation;

void DarkChampion::StateIdle::onEnter() {}
void DarkChampion::StateIdle::onUpdate(float dt) {}
void DarkChampion::StateIdle::onLeave() {}

void DarkChampion::StateMoving::onEnter() {
  owner->animation_timer.setClip(0);
  owner->destination = owner->getFurthestFacingPointOnPlatform();
  AI_DEBUG_OUT("Entered DarkChampionMoveState");
  if((owner->getPosition() - owner->destination).fuzzyZero()) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();    
    if((owner->getPosition() - owner->destination).fuzzyZero()) {
      //owner->changeDirection();
      owner->changeState(new DarkChampion::StateIdle(owner));
    }
  }
}
void DarkChampion::StateMoving::onUpdate(float dt) {
  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 linear_velocity = owner->getRigidBody()->getLinearVelocity();
  btVector3 current_position = owner->getPosition();
  owner->hung = false;
  if((owner->previous_position - current_position).length() <= 0.01f) {
    owner->hung = true;
  }
  owner->previous_position = current_position;
  if(owner->hung == true) {
    owner->hang_timer += dt;
  }
  else {
    owner->hang_timer = 0.0f;
  }
  
  if(owner->hang_timer > MAX_HANG_TIME) {
    owner->changeDirection();
    owner->changeState(new DarkChampion::StateMoving(owner));
    owner->hang_timer = 0.0f;
    return;
  }

  if(current_position.distance(owner->destination) <= in_range_eps) {
    owner->changeDirection();
    owner->destination = owner->getFurthestFacingPointOnPlatform();
  }
  if(owner->lookForPlayer(btVector3(owner->getDirection() * VIEW_RANGE, 0, 0))) {
    owner->changeState(new DarkChampion::StateAttack(owner));
  }
  linear_velocity.setX(owner->getDirection() * DarkChampion::MOVE_SPEED);
  owner->getRigidBody()->setLinearVelocity(linear_velocity);
}
void DarkChampion::StateMoving::onLeave() {}





void DarkChampion::StateAttack::onEnter() {
  AI_DEBUG_OUT("Entered DarkChampion::StateAttack"); 
  owner->animation_timer.setClip(3);
  owner->substate = CHARGING;
}
void DarkChampion::StateAttack::onUpdate(float dt) {
  btVector3 linear_velocity = owner->getRigidBody()->getLinearVelocity();
  switch(owner->substate) {
  case(CHARGING):
    {
      if(!owner->lookForPlayer(btVector3(owner->getDirection() * VIEW_RANGE, 0, 0))) {
	owner->changeState(new DarkChampion::StateMoving(owner));
	return;
      }
      else if(owner->lookForPlayer(btVector3(owner->getDirection() * ATTACK_RANGE, 0, 0))) {
	owner->animation_timer.setClip(1);
	owner->substate = SWINGING;  
      }
      else if(owner->atPlatformEnd()) {	
	linear_velocity.setX(0.0f);
      }
      else {
	linear_velocity.setX(owner->getDirection() * CHARGE_SPEED);
      }
    break;
    }
  case(SWINGING):
    {
      if(!owner->lookForPlayer(btVector3(owner->getDirection() * ATTACK_RANGE, 0, 0))) {
	owner->animation_timer.setClip(3);
	owner->substate = CHARGING;
      }
      else {
        linear_velocity.setX(0.0f);
      }
      break;
    }
  }
  owner->getRigidBody()->setLinearVelocity(linear_velocity);
}
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

DarkChampion::DarkChampion(GameState* gamestate, const btVector3& position) 
  : StateMachineObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme, new DarkChampion::StateMoving(this)), health(INITIAL_HEALTH), can_attack(true), hang_timer(0.0f) {
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
