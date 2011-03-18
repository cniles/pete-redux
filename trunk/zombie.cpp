#include "zombie.h"
#include "draw.h"
#include <iostream>

Animation Zombie::animation = Animation();

void ZombieStateDead::onEnter() {}
void ZombieStateDead::onUpdate(float) {}
void ZombieStateDead::onLeave() {}

void ZombieStateDying::onEnter() {}
void ZombieStateDying::onUpdate(float dt) {
  if(ai->getOwner()->getAnimationState() == AnimationTimer::STOPPED) {
    ai->changeState(new ZombieStateDead(ai));
  }
}
void ZombieStateDying::onLeave() {}

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
void ZombieStateMoving::onLeave() {}
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

void ZombieStateIdle::onEnter() {}
void ZombieStateIdle::onUpdate(float dt) {}
void ZombieStateIdle::onLeave() {}

Zombie::Zombie(float x, float y, GameState* gamestate) 
  : GameObject(x,y, gamestate, COL_ENEMY, COL_LEVEL | COL_PLAYER),
    ai(PlatformAI(gamestate, this)), health(INITIAL_HEALTH) {
  
  if(animation.getClipCount()==0) {
    animation = Animation("gfx/zombie");
  }

  animation_timer = AnimationTimer(&animation);
  animation_timer.start();
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

void Zombie::takeDamage(int damage, int type) {
  if(health > 0) {
    health -= damage;
    if(health <= 0) {
      changeCollisionScheme(COL_ENEMY,COL_LEVEL);
      animation_timer.playClipOnce(2, -1);
      ai.changeState(new ZombieStateDying(&ai));
    }
  }
}
