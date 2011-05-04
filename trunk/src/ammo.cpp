#include "draw.h"
#include "ammo.h"

#include "gamestate.h"

Animation Ammo::animation = Animation();

Ammo::Ammo(GameState* gamestate, btVector3 position) 
  : PhysicsObject(gamestate, position, &animation, default_collision_scheme), rotation(0) {
}

Ammo::~Ammo () {
}

void Ammo::loadStaticAssets() {
  animation = Animation("gfx/ammo");
}

void Ammo::update(float dt) {
  rotation += rotation_speed * dt;
  rotation %= 360;
  if(collisionWithPlayerDetected()) {
    gamestate->player->increaseAmmo(AMMO_REWARD);
    disableDraw();
    //flagForRemoval();
    removeCollisionBody();
  }
}

void Ammo::draw() {
  if(disable_draw) return;
  glPushMatrix();

  glTranslatef(position.getX(),position.getY(),-0.5f);

  glRotatef(rotation, 0.0f, 1.0f, 0.0f);
  glScalef(0.5f, 0.5f, 0.5f);
  glTranslatef(-0.5f, -0.5f, 0.0f);

  glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, 0));
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.5f);
  drawQuad(); 
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, 0));
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -0.5f);
  drawQuadFlip(); 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  drawQuadFlip();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(1.0f, 0.0f, 0.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  drawQuad();
  glPopMatrix();
 
  glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, 1));
  glPushMatrix();
  glTranslatef(0.0f, 1.0f, 0.5f);
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  drawQuad();
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, 2));
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -0.5f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  drawQuad();
  glPopMatrix();
  glPopMatrix();
}
