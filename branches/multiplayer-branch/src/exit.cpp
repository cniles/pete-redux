#include "exit.h"

#include "gamestate.h"

Animation Exit::animation = Animation();

CollisionScheme Exit::exit_collision_scheme(COL_ENEMY, COL_LEVEL, btScalar(0.0f), &PhysicsObject::default_collision_shape);

Exit::Exit(GameState* gamestate, const btVector3& position) 
  : PhysicsObject(gamestate, position, &animation, exit_collision_scheme) {
  changeDirection();
}

Exit::~Exit () {
}

void Exit::loadStaticAssets() {
  animation = Animation("gfx/exit");
}

void Exit::update(float dt) {
  animation_timer.tick(dt * 1000.0f);
  if((int)gamestate->player->getX() == (int)position.getX() && (int)gamestate->player->getY() == (int)position.getY()) {
    gamestate->exit_toggled = true;
  }
}
