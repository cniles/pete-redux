#include "zombie.h"
#include "draw.h"
#include <iostream>


Animation Zombie::animation = Animation();

ZombieStateMoving::ZombieStateMoving(PlatformAI* ai) 
  : State(ai) {
}

void ZombieStateMoving::onEnter() {
  btVector3 destination = ai->getFurthestFacingPointOnPlatform();
  
  if((ai->getOwner()->getPosition() - destination).fuzzyZero()) {
    ai->getOwner()->changeDirection();
    destination = ai->getFurthestFacingPointOnPlatform();
    
    if((ai->getOwner()->getPosition() - destination).fuzzyZero()) {
      ai->getOwner()->changeDirection();
      ai->changeState(new ZombieStateIdle(ai));
    }
  }
  ai->setDestination(destination);    
}

void ZombieStateMoving::onLeave() {
}

void ZombieStateMoving::onUpdate(float dt) { 
  GameObject* owner = ai->getOwner();

  // Check if object is near it's destination & turn it around if so
  const static float in_range_eps = 0.1f;
  btVector3 destination = ai->getDestination();
  btVector3 current_position = owner->getPosition();
  if(current_position.distance(destination) <= in_range_eps) {
    owner->changeDirection();
    ai->setDestination(ai->getFurthestFacingPointOnPlatform());
  }

  // Nudge the object in the correct direction.
  btVector3 force(owner->getDirection(), 0.0f, 0.0f);
  force = force * 3;
  btRigidBody* body = owner->getRigidBody();
  body->applyCentralForce(force);
}

void ZombieStateIdle::onEnter() {
}

void ZombieStateIdle::onLeave() {
}

ZombieStateIdle::ZombieStateIdle(PlatformAI* ai) 
  : State(ai) {
}

void ZombieStateIdle::onUpdate(float dt) {

}

Zombie::Zombie(float x, float y, GameState* gamestate) 
  : GameObject(x,y, gamestate, COL_ENEMY, COL_LEVEL | COL_PLAYER) , animation_timer(AnimationTimer(&animation)) {
  if(animation.getClipCount()==0) {
    animation = Animation("gfx/zombie");
  }
  animation_timer.start();
  ai = PlatformAI(gamestate, this);
  ai.changeState(new ZombieStateMoving(&ai));
}

void Zombie::draw() {
  glPushMatrix();
  glTranslatef(position.getX(),position.getY(),-0.5f);
  glTranslatef(-0.5f, -0.5f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, animation_timer.getFrame());
  if(direction==-1) {
    drawQuadFlip();
  }
  else {
    drawQuad();
  }
  glPopMatrix();
}

void Zombie::update(float dt) {
  ai.update(dt) ;
  animation_timer.tick(dt*1000);
}
