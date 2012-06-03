#include "medpack.h"

#include "gamestate.h"

const float Medpack::MASS = 1.0f;

Animation Medpack::animation = Animation();

CollisionScheme Medpack::collision_scheme(COL_ENEMY, COL_LEVEL | COL_PLAYER, Medpack::MASS, &default_collision_shape);

Medpack::Medpack(GameState* gamestate, const btVector3& position) 
  : PhysicsObject(gamestate, position, &animation, Medpack::collision_scheme) {
  rigid_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}

Medpack::~Medpack () {
}

void Medpack::loadStaticAssets() {
  animation = Animation("gfx/medpack");
}

void Medpack::update(float dt) {
  animation_timer.tick(dt * 1000.0f);
  if(collisionWithPlayerDetected()) {
    gamestate->player->increaseHealth(HEALTH_REWARD);
    disableDraw();
    //flagForRemoval();
    removeCollisionBody();
  }
}
