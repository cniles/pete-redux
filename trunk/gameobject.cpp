#include <iostream>

#include "gameobject.h"
#include "draw.h"

GameObject::GameObject(GameState* gamestate, btVector3 position, Animation* animation)
  : gamestate(gamestate), animation_timer(AnimationTimer(animation)), 
    position(position), direction(-1) {
  animation_timer.start();
}


void GameObject::takeDamage(int damage, int type) {
}

GameObject::~GameObject() {
}

void GameObject::draw() {
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
