#include "gameobject.h"

GameObjectMotionState::GameObjectMotionState(const btTransform& initial_position, GameObject* object) 
  : position(initial_position), object(object) {
  
}

GameObjectMotionState::~GameObjectMotionState() {

}

void GameObjectMotionState::getWorldTransform(btTransform& world_transform) {
  world_transform = position;
}

void GameObjectMotionState::setWorldTransform(const btTransform& world_transform) {
  
}


GameObject::GameObject(float x, float y) {
  position = btVector3(x, y, 0);
  direction = -1;
}
