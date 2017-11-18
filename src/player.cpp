#include <iostream>
#include <cmath>
#include "gamestate.h"
#include "player.h"
#include "draw.h"
#include "physicsobject.h"

Animation Player::animation = Animation();

btCapsuleShape Player::child_collision_shape(PLAYER_COLLISION_WIDTH, PLAYER_COLLISION_HEIGHT);
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
  jumping = false;
  on_ground = false;
  hit_head = false;
  can_shoot = true;
  ctrl_released = true;

  position = btTransform(btQuaternion(0,0,0,1), btVector3(tile_x+0.5f, tile_y+0.5f, 0));
  health = PLAYER_INITIAL_HEALTH;
  ammo = PLAYER_INITIAL_AMMO;
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

void Player::applyJumpVelocity() {
  btVector3 velocity = rigid_body->getLinearVelocity();
  velocity.setY(PLAYER_JUMP_VELOCITY);
  rigid_body->setLinearVelocity(velocity);
}

void Player::handleKeyStates(const KeyStates& key_states) {
  float x_acceleration = isOnGround()? PLAYER_ACCELERATION : PLAYER_IN_AIR_ACCELERATION;
  if(key_states.right_held) {
    rigid_body->activate(true);
    flip_sprite = false;
    direction = 1;
    rigid_body->applyCentralForce(btVector3(x_acceleration, 0.0f, 0.0f));
  }
  else if(key_states.left_held) {
    rigid_body->activate(true);
    flip_sprite = true;
    direction = -1;
    rigid_body->applyCentralForce(btVector3(-x_acceleration, 0.0f, 0.0f));
  }

  if(key_states.space_held && !jumping) {
    if(isOnGround()) {
      jumping = true;
      jump_timer = 0;
    }
  }
  
  if(key_states.ctrl_held) {
    if(can_shoot && ammo > 0) {
      ammo--;
      fireShotgun();
      can_shoot = false;
      shoot_timer = PLAYER_SHOOT_PAUSE_TIME;
    }
  }
  else {
    ctrl_released = true;
  }
  
};

void Player::sync(const Player* player) {
  ammo = player->ammo;
  health = player->health;
}

void Player::update(Uint32 dt, const KeyStates& key_states) {
  calcIsOnGround();
  calcHitHead();
  handleKeyStates(key_states);
  animation_timer.tick(dt);

  float dtf = float(dt)/1000.0f;
  if(!can_shoot) {
    shoot_timer -= dtf;
    if(shoot_timer <= 0) {
      can_shoot = true;
    }
  }
 
  if(hasHitHead()) {
    jumping = false;
  }

  if(jumping) {
    if(key_states.space_held) {
      if(jump_timer < PLAYER_JUMP_MAX_TIME) {
	applyJumpVelocity();
      }
    }
    else if(jump_timer < PLAYER_JUMP_MIN_TIME) {
      applyJumpVelocity();
    }
    else {
      jumping = false;
    }

    jump_timer += dtf;
    if(jump_timer >= PLAYER_JUMP_MAX_TIME) {
      jumping = false;
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

void Player::calcHitHead() {
  hit_head = false;
  btVector3 origin = position.getOrigin(); 
  for(int i = -1; i <= 1; i++) {
    btVector3 leg(0.1 * i, (PLAYER_COLLISION_HEIGHT) , 0.0f);
    leg += origin;
    btCollisionWorld::ClosestRayResultCallback callback(origin, leg);
    gamestate->dynamics_world->rayTest(origin, leg, callback);
    if(callback.hasHit()) {
      if(callback.m_collisionObject) {
	if(callback.m_collisionObject->getUserPointer() != this) {
	  hit_head = true;
	  return;
	}
      }
    }
  }
} 

void Player::calcIsOnGround() {
  on_ground = false;
  btVector3 origin = position.getOrigin(); 
  for(int i = -1; i <= 1; i++) {
    btVector3 leg(0.1 * i, (-PLAYER_COLLISION_HEIGHT * (1 + PLAYER_ON_GROUND_TOLERANCE)) , 0.0f);
    leg += origin;
    btCollisionWorld::ClosestRayResultCallback callback(origin, leg);
    gamestate->dynamics_world->rayTest(origin, leg, callback);
    if(callback.hasHit()) {
      if(callback.m_collisionObject) {
	if(callback.m_collisionObject->getUserPointer() != this) {
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

bool Player::hasHitHead() {
  return hit_head;
}

void Player::fireShotgun() {
  animation_timer.playClipOnce(1, 0);
  btVector3 origin = position.getOrigin(); 
  for(int i = 0; i < 3; i++) {
    btVector3 bullet_path(direction * 5.0, i*0.5f, 0.0f);
    bullet_path += origin;
    btCollisionWorld::ClosestRayResultCallback callback(origin, bullet_path);
    gamestate->dynamics_world->rayTest(origin, bullet_path, callback);
    if(callback.hasHit()) {
      if(callback.m_collisionObject) {
	PhysicsObject* object = (PhysicsObject*)callback.m_collisionObject->getUserPointer();
	if(object) {
	  object->notifyWasShot(1, 0);
	  btRigidBody* body = const_cast<btRigidBody*>(btRigidBody::upcast(callback.m_collisionObject));
	  if(body) {
	    body->applyCentralForce(btVector3(direction*PLAYER_SHOTGUN_FORCE, 0.0f, 0.0f));
	  }
	}
      }
    }
  }
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

void Player::increaseAmmo(int amount) {
  ammo += amount;
}

void Player::increaseHealth(int amount) {
  health += amount;
}
