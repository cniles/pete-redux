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
  btVector3 current_position = ai->getOwner()->getPosition();
  btVector3 velocity(ai->getOwner()->getDirection() * dt, 0.0f, 0.0f);
  btVector3 destination = ai->getDestination();

  current_position += velocity;

  if(current_position.distance(destination) <= velocity.length()) {
    ai->getOwner()->setPosition(destination);
    ai->getOwner()->changeDirection();
    ai->setDestination(ai->getFurthestFacingPointOnPlatform());
  }
  else {
    ai->getOwner()->setPosition(current_position);
  }
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
  : GameObject(x,y, gamestate) , animation_timer(AnimationTimer(&animation)) {
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
