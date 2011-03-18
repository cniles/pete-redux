#include "gamestate.h"
#include "gameobject.h"

btCapsuleShape GameObject::child_default_collision_shape(0.25, 0.4);
btConvex2dShape GameObject::default_collision_shape(&GameObject::child_default_collision_shape);

GameObjectMotionState::GameObjectMotionState(const btTransform& initial_position, GameObject* object) 
  : position(initial_position), object(object) {
  
}

GameObjectMotionState::~GameObjectMotionState() {

}

void GameObjectMotionState::getWorldTransform(btTransform& world_transform) const {
  world_transform = position;
}

void GameObjectMotionState::setWorldTransform(const btTransform& world_transform) {
  position = world_transform;
  object->setPosition(position.getOrigin());
}

GameObject::GameObject(float x, float y, GameState* gamestate, int collision_mask_type, int collision_flags, btCollisionShape* collision_shape) {
  this->gamestate = gamestate;
  direction = -1;
  position = btVector3(x+0.5f, y+0.5f, 0);

  btMotionState* motion_state = 
    new GameObjectMotionState(btTransform(btQuaternion(0,0,0,1),position), this);
  btScalar mass = 1.0;
  btVector3 inertia(0,0,0);
  collision_shape->calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, &default_collision_shape, inertia);
  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  gamestate->dynamics_world->addRigidBody(rigid_body, collision_mask_type, collision_flags);
}

void GameObject::changeCollisionScheme(int group, int flags, btCollisionShape* shape) {
  btVector3 linear_velocity = rigid_body->getLinearVelocity();

  delete rigid_body->getMotionState();
  gamestate->dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body;
  
  btMotionState* motion_state = 
    new GameObjectMotionState(btTransform(btQuaternion(0,0,0,1),position), this);
  btScalar mass = 1.0;
  btVector3 inertia(0,0,0);
  shape->calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, shape, inertia);
  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  rigid_body->setLinearVelocity(linear_velocity);
  gamestate->dynamics_world->addRigidBody(rigid_body, group, flags);
  
}

void GameObject::takeDamage(int damage, int type) {
}

void GameObject::removeCollisionBody() {
  gamestate->dynamics_world->removeRigidBody(rigid_body);
}

GameObject::~GameObject() {
  delete rigid_body->getMotionState();
  gamestate->dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body;
}
