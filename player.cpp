#include <iostream>
#include <cmath>
#include "gamestate.h"
#include "player.h"
#include "draw.h"
#include "physicsobject.h"

Animation Player::animation = Animation();

btCapsuleShape Player::child_collision_shape(0.25, 0.4);
btConvex2dShape Player::collision_shape(&Player::child_collision_shape);

Player::Player(int tile_x, int tile_y, GameState* gamestate) {
  if(animation.getClipCount() == 0) {
    animation = Animation("gfx/player");
  }
  this->gamestate = gamestate;
  animation_timer = AnimationTimer(&animation);
  animation_timer.start();

  shoot_timer = 0.0f;

  flip_sprite = false;
  space_held = false;
  on_ground = false;
  can_shoot = true;
  ctrl_released = true;

  position = btTransform(btQuaternion(0,0,0,1), btVector3(tile_x+0.5f, tile_y+0.5f, 0));
  direction = 1;
  btMotionState* motion_state = 
    new PlayerMotionState(position, this);
  btScalar mass = 0.7;
  btVector3 inertia(0,0,0);
  collision_shape.calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, &collision_shape, inertia);
  rigid_body = new btRigidBody(rigid_body_ci);
  rigid_body->setAngularFactor(btScalar(0));
  rigid_body->setActivationState(DISABLE_DEACTIVATION);
  rigid_body->setUserPointer((void*)this);
  gamestate->dynamics_world->addRigidBody(rigid_body,  COL_PLAYER, COL_LEVEL | COL_ENEMY);
}

Player::~Player() {
  delete rigid_body->getMotionState();
  gamestate->dynamics_world->removeRigidBody(rigid_body);
  delete rigid_body;
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
    direction = 1;
    rigid_body->applyCentralForce(btVector3(x_acceleration, 0.0f, 0.0f));
  }
  else if(key_states.left_held) {
    flip_sprite = true;
    direction = -1;
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
  else if(key_states.space_held == false){
    space_held = false;
  }

  if(key_states.ctrl_held) {
    if(can_shoot) {
      fireShotgun();
      can_shoot = false;
      shoot_timer = SHOOT_PAUSE_TIME;
    }
  }
  else {
    ctrl_released = true;
  }
  
};

void Player::update(Uint32 dt, const KeyStates& key_states) {
  calcIsOnGround();
  handleKeyStates(key_states);
  animation_timer.tick(dt);

  float dtf = float(dt)/1000.0f;
  if(!can_shoot) {
    shoot_timer -= dtf;
    if(shoot_timer <= 0) {
      can_shoot = true;
    }
  }

  btTransform transform;
  btVector3 origin;
  rigid_body->getMotionState()->getWorldTransform(transform);
  origin = transform.getOrigin();
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

void Player::calcIsOnGround() {
  on_ground = false;
  btDispatcher* dispatcher = gamestate->dynamics_world->getDispatcher();
  int num_manifolds = dispatcher->getNumManifolds();
  for(int i = 0; i < num_manifolds; i++) {
    btPersistentManifold* contact_manifold = dispatcher->getManifoldByIndexInternal(i);
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

void Player::fireShotgun() {
  animation_timer.playClipOnce(1, 0);
  btVector3 origin = position.getOrigin(); 
  for(int i = 0; i < 3; i++) {
    btVector3 bullet_path(direction * 5.0, i*0.5f, 0.0f);
    bullet_path += origin;
    ShotgunCallback callback(direction);
    gamestate->dynamics_world->rayTest(origin, bullet_path, callback);
  }
}

ShotgunCallback::ShotgunCallback(int direction) : direction(direction) {}

btScalar ShotgunCallback::addSingleResult(btCollisionWorld::LocalRayResult& ray_result, bool normal_in_world_space) {
  btVector3 source_vector(direction * 100.0f, 0.0f, 0.0f);
  btRigidBody* body = dynamic_cast<btRigidBody*>(ray_result.m_collisionObject);
  PhysicsObject* target = (PhysicsObject*)body->getUserPointer();
  if(target) {
    target->notifyWasShot(1, 0);
  }
  body->applyCentralForce(source_vector);
  return btScalar(0.0f);
}

PlayerMotionState::PlayerMotionState(const btTransform& initial_position, Player* player) 
  : position(initial_position), player(player) {
}

PlayerMotionState::~PlayerMotionState() {
}

void PlayerMotionState::getWorldTransform(btTransform& world_transform) const {
  world_transform = position;
}

void PlayerMotionState::setWorldTransform(const btTransform& world_transform) {
  position = world_transform;
  player->setPosition(position);
}

void Player::takeDamage(int damage, int type) {
  health -= damage;
}
