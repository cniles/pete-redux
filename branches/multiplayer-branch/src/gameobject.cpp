#include <iostream>

#include "gameobject.h"
#include "draw.h"

GameObject::GameObject(GameState* gamestate, const btVector3& position, Animation* animation)
  : gamestate(gamestate), animation(animation), animation_timer(AnimationTimer(animation)), 
    position(position), direction(-1), disable_draw(false) {
  DEBUG_OUT("Create GameObject");
  animation_timer.start();
}


GameObject::~GameObject() {

}

void GameObject::draw() {
  if(disable_draw) return;
  float width = (float)animation->getWidth();
  float height = (float)animation->getHeight();
  float x_aspect = width / height;
  
  glPushMatrix();
  glTranslatef(position.getX(),position.getY(),-0.5f);
  glTranslatef(-0.5f, -0.5f, 0.0f);
  glScalef(x_aspect, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, animation_timer.getFrame());
  if(direction==-1) {
    drawQuadFlip();
  }
  else {
    drawQuad();
  }
  glPopMatrix();
}

