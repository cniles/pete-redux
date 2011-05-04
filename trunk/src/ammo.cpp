#include "draw.h"
#include "ammo.h"
#include "gamestate.h"

Animation Ammo::animation;

Ammo::Ammo(GameState* gamestate, btVector3 position) 
  : PhysicsObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme) {
}

void Ammo::loadStaticAssets() {
  animation = Animation("gfx/ammo");
}

void Ammo::update(float dt) {
  animation_timer.tick(dt*1000.0f);
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
  glTranslatef(-0.5f, -0.5f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, animation_timer.getFrame());
  drawCube();
  glPopMatrix();
}
