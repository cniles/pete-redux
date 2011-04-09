 #include "darkchampion.h"
Animation DarkChampion::animation;

void DarkChampion::StateIdle::onEnter() {}
void DarkChampion::StateIdle::onUpdate(float dt) {}
void DarkChampion::StateIdle::onLeave() {}

void DarkChampion::StateMove::onEnter() {}
void DarkChampion::StateMove::onUpdate(float dt) {}
void DarkChampion::StateMove::onLeave() {}

void DarkChampion::StateDead::onEnter() {}
void DarkChampion::StateDead::onUpdate(float dt) {}
void DarkChampion::StateDead::onLeave() {}

DarkChampion::DarkChampion(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme, new DarkChampion::StateIdle(this)) {
  animation_timer.setClip(2);
}

void DarkChampion::loadStaticAssets() {
  animation = Animation("gfx/darkchampion");
}
