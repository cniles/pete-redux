#include "medpack.h"

#include "gamestate.h"

Animation Medpack::animation = Animation();

Medpack::Medpack(GameState* gamestate, btVector3 position) 
  : PhysicsObject(gamestate, position, &animation, default_collision_scheme)            {
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
