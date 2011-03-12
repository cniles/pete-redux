#include <iostream>
#include <math.h>
#include "gamestate.h"
#include "player.h"
#include "draw.h"

Animation Player::animation = Animation();

void Player::calcIsOnGround() {
  on_ground = false;
  int num_manifolds = gamestate->bullet.dispatcher->getNumManifolds();
  for(int i = 0; i < num_manifolds; i++) {
    btPersistentManifold* contact_manifold = gamestate->bullet.dispatcher->getManifoldByIndexInternal(i);
    btCollisionObject* obj_a = static_cast<btCollisionObject*>(contact_manifold->getBody0());
    btCollisionObject* obj_b = static_cast<btCollisionObject*>(contact_manifold->getBody1());
    if(obj_a->getUserPointer() == (void*)this || obj_b->getUserPointer() == (void*)this) {
      int num_contacts = contact_manifold->getNumContacts();
      if(num_contacts) {
	btVector3 average_a(0.0f, 0.0f, 0.0f);
	btVector3 average_b(0.0f, 0.0f, 0.0f);
	for(int j = 0; j < num_contacts; j++) {
	btManifoldPoint& pt = contact_manifold->getContactPoint(j);
	average_a += pt.getPositionWorldOnA();
	average_b += pt.getPositionWorldOnB();
	}
	
	float final_x = ((average_a.getX() + average_b.getX())/num_contacts) * 0.5f;
	float final_y = ((average_a.getY() + average_b.getY())/num_contacts) * 0.5f;
	float final_z = ((average_a.getZ() + average_b.getZ())/num_contacts) * 0.5f;
	
	contact_manifold->clearManifold();
	
	btTransform trans;
	rigid_body->getMotionState()->getWorldTransform(trans);
	float body_y = trans.getOrigin().getY();
	float body_x_rel = final_x - trans.getOrigin().getX();
	if(final_y < body_y && body_x_rel < 0.2f && body_x_rel > -0.2f) {
	  on_ground = true;
	  return;
	}
      }
    }
  }  
}

bool Player::isOnGround() {
  return on_ground;
}

Player::Player(int x, int y, GameState* gamestate) {
  if(animation.getClipCount() == 0) {
    animation = Animation("gfx/player");
  }
  this->gamestate = gamestate;
  animation_timer = AnimationTimer(&animation);
  animation_timer.start();


  flip_sprite = false;
  space_held = false;
  on_ground = false;

  initial_position = btVector3(x+0.5f, y+0.5f, 0);
  collision_shape = new btCapsuleShape(0.25, 0.4);
  btDefaultMotionState* motion_state = 
    new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),initial_position));
  btScalar mass = 0.8;
  btVector3 inertia(0,0,0);
  collision_shape->calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, collision_shape, inertia);
  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  gamestate->bullet.dynamics_world->addRigidBody(rigid_body);
}

Player::~Player() {
  gamestate->bullet.dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body->getMotionState();
  delete rigid_body;
  delete collision_shape;
}

void Player::operator=(const Player& other) {
  gamestate = other.gamestate;
}

void Player::draw() const {
  btTransform transform;
  glPushMatrix();
  rigid_body->getMotionState()->getWorldTransform(transform);
  glTranslatef(transform.getOrigin().getX()-0.5f, transform.getOrigin().getY()-0.5f, -0.5f);
  glBindTexture(GL_TEXTURE_2D, animation_timer.getFrame());
  if(flip_sprite) {
    drawQuadFlip();
  }
  else {
    drawQuad();
  }
  glPopMatrix();
}

#define PLAYER_JUMP_VELOCITY 7.0
#define PLAYER_ACCELERATION 10
#define PLAYER_IN_AIR_ACCELERATION 3
void Player::handleKeyStates(const KeyStates& key_states) {
  float x_acceleration = isOnGround()? PLAYER_ACCELERATION : PLAYER_IN_AIR_ACCELERATION;
  if(key_states.right_held) {
    flip_sprite = false;
    rigid_body->applyCentralForce(btVector3(x_acceleration, 0.0f, 0.0f));
  }
  else if(key_states.left_held) {
    flip_sprite = true;
    rigid_body->applyCentralForce(btVector3(-x_acceleration, 0.0f, 0.0f));
  }

  if(key_states.space_held && !space_held) {
    if(isOnGround()) {
      space_held = true;
      btVector3 velocity = rigid_body->getLinearVelocity();
      if(velocity.getY() < PLAYER_JUMP_VELOCITY) { // prevent slowdown from jumping
	velocity.setY(PLAYER_JUMP_VELOCITY);
	rigid_body->setLinearVelocity(velocity);
      }
    }
  }
  if(key_states.space_held==false) {
    space_held = false;
  }
};

void Player::update(Uint32 dt, const KeyStates& key_states) {
  calcIsOnGround();
  handleKeyStates(key_states);
  animation_timer.tick(dt);
  float dtf = float(dt)/1000.0f;
}

float Player::getX() {
  btTransform transform;
  rigid_body->getMotionState()->getWorldTransform(transform);
  return transform.getOrigin().getX();
}

float Player::getY() {
  btTransform transform;
  rigid_body->getMotionState()->getWorldTransform(transform);
  return transform.getOrigin().getY();
}
