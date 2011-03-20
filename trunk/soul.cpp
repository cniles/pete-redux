#include "soul.h"

Animation Soul::animation;

void Soul::StateIdle::onEnter() {}
void Soul::StateIdle::onUpdate(float dt) {}
void Soul::StateIdle::onLeave() {}

void Soul::StateMove::onEnter() {}
void Soul::StateMove::onUpdate(float dt) {
  if(owner->rigid_body->getLinearVelocity().fuzzyZero()) {
    owner->changeDirection();
    btVector3 force(owner->direction, 0.0f, 0.0f);
    owner->rigid_body->setLinearVelocity(force);
  }
}
void Soul::StateMove::onLeave() {}

void Soul::StateDead::onEnter() {
  CollisionScheme scheme = PhysicsObject::default_collision_scheme;
  scheme.collision_flags = COL_LEVEL;
  owner->changeCollisionScheme(scheme);
  owner->animation_timer.playClipOnce(2, -1);
  owner->rigid_body->setGravity(btVector3(0.0, 0.0, 0.0));
}
void Soul::StateDead::onUpdate(float dt) {
  if(owner->animation_timer.getState() == AnimationTimer::STOPPED) {
    owner->disableDraw();
    owner->stopUpdate();
    owner->removeCollisionBody();
  }
}
void Soul::StateDead::onLeave() {}

Soul::Soul(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme, new Soul::StateMove(this)), 
    health(Soul::INITIAL_HEALTH) {
  rigid_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

void Soul::notifyWasShot(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      changeState(new Soul::StateDead(this));
    }
  }
}

void Soul::loadStaticAssets() {
  animation = Animation("gfx/soul");
}
