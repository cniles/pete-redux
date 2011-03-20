#include "bat.h"
#include "physicsobject.h"

void Bat::StateMove::onEnter() {
  owner->rigid_body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}
void Bat::StateMove::onUpdate(float dt) {}
void Bat::StateMove::onLeave() {}

void Bat::StateDead::onEnter() {}
void Bat::StateDead::onUpdate(float dt) {}
void Bat::StateDead::onLeave() {}

void Bat::StateAttack::onEnter() {}
void Bat::StateAttack::onUpdate(float dt) {}
void Bat::StateAttack::onLeave() {}

//Bat::State::onLeave() {}
//Bat::State::onUpdate() {}
//Bat::State::onLeave() {}

btBox2dShape Bat::collision_shape(btVector3(0.2f, 0.2f, 0.1f));
CollisionScheme Bat::collision_scheme(COL_ENEMY, COL_LEVEL | COL_PLAYER, Bat::MASS, &Bat::collision_shape);
Animation Bat::animation;


Bat::Bat(GameState* gamestate, btVector3 position)
  : StateMachineObject(gamestate, position, &animation, Bat::collision_scheme, new Bat::StateMove(this)) {
}

void Bat::loadStaticAssets() {
  animation = Animation("gfx/bat");
}
