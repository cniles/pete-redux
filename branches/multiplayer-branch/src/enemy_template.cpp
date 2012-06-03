//#include "enemy.h"
#include "enemy_template.h"
Animation Enemy::animation;

void Enemy::StateIdle::onEnter() {}
void Enemy::StateIdle::onUpdate(float dt) {}
void Enemy::StateIdle::onLeave() {}

void Enemy::StateMove::onEnter() {}
void Enemy::StateMove::onUpdate(float dt) {}
void Enemy::StateMove::onLeave() {}

void Enemy::StateDead::onEnter() {}
void Enemy::StateDead::onUpdate(float dt) {}
void Enemy::StateDead::onLeave() {}

Enemy::Enemy(GameState* gamestate, btVector3 position) 
  : StateMachineObject(gamestate, position, &animation, PhysicsObject::default_collision_scheme, new Enemy::StateIdle(this)) {
}

void Enemy::loadStaticAssets() {
  animation = Animation("gfx/enemy");
}
