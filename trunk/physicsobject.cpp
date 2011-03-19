#include <iostream>
#include "physicsobject.h"
#include "gamestate.h"

btCapsuleShape PhysicsObject::child_default_collision_shape(0.25, 0.4);
btConvex2dShape PhysicsObject::default_collision_shape(&PhysicsObject::child_default_collision_shape);
CollisionScheme PhysicsObject::default_collision_scheme(COL_ENEMY, COL_LEVEL | COL_PLAYER, btScalar(1.0), &PhysicsObject::default_collision_shape);

PhysicsObject::PhysicsObject(GameState* gamestate, btVector3 position, Animation* animation, CollisionScheme scheme)
  : GameObject(gamestate, position, animation) {
  btMotionState* motion_state = new GameObjectMotionState(btTransform(btQuaternion(0,0,0,1), position), this);
  btVector3 inertia(0,0,0);
  scheme.collision_shape->calculateLocalInertia(scheme.mass, inertia);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(scheme.mass, motion_state, scheme.collision_shape, inertia);
  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  gamestate->dynamics_world->addRigidBody(rigid_body, scheme.collision_group, scheme.collision_flags);
}

PhysicsObject::~PhysicsObject() {
  delete rigid_body->getMotionState();
  gamestate->dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body;
}

void PhysicsObject::removeCollisionBody() {
  gamestate->dynamics_world->removeRigidBody(rigid_body);
}

void PhysicsObject::changeCollisionScheme(CollisionScheme scheme) {
  btVector3 linear_velocity = rigid_body->getLinearVelocity();

  delete rigid_body->getMotionState();
  gamestate->dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body;
  
  btMotionState* motion_state = 
    new GameObjectMotionState(btTransform(btQuaternion(0,0,0,1),position), this);
  btVector3 inertia(0,0,0);
  scheme.collision_shape->calculateLocalInertia(scheme.mass,inertia);

  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(scheme.mass, motion_state, scheme.collision_shape, inertia);

  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  rigid_body->setLinearVelocity(linear_velocity);

  gamestate->dynamics_world->addRigidBody(rigid_body, scheme.collision_group, scheme.collision_flags);  
}

CollisionScheme::CollisionScheme(int group, int flags, btScalar mass, btCollisionShape* shape)
  : collision_group(group), collision_flags(flags), mass(mass), collision_shape(shape) {
}

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
