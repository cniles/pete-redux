#include "zombie.h"
#include "gamestate.h"
#include <iostream>
GameState::GameState(Level level)
  : level(level) {
  initialize();
}

GameState::~GameState() {
  delete player;
  for(int i = 0; i < objects.size(); i++) {
    delete objects[i];
  }
  for(int i = 0; i < level_bodies.size(); i++) {
    delete level_bodies[i]->getMotionState();
    delete level_bodies[i];
    delete level_shapes[i];
  }
}

void GameState::initialize() {
  GameObjectToken player_position = level.getObject(30);
  player = new Player(player_position.x, player_position.y, this);
  
  TokenList::const_iterator token_iter = level.getTokensStart();
  while(token_iter != level.getTokensEnd()) {
    switch(token_iter->id) {
    case 0:
      objects.push_back(new Zombie(token_iter->x, token_iter->y, this));
      break;
    }
    token_iter++;
  }

  for(int y = 0; y < 10; y++) {
    int x = 0;
    while(x < level.getLength()) {
      if(level.getTile(x,y)!=0) {
	int stop_x = x;
	while(level.getTile(stop_x,y) != 0 && stop_x < level.getLength() && stop_x-x < 40 ) {
	  stop_x++;
	}
	float width = stop_x - x;
	float center_x = width/2.0f + x;
	// Add a static rigid-body for each contiguous horizontal platform.
	btCollisionShape* collision_shape = new btBoxShape(btVector3(width/2.0f,0.5f,0.5f));
	btDefaultMotionState* motion_state = 
	  new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(center_x, y+0.5f, 0)));
	btScalar mass = 0;
	btVector3 inertia(0,0,0);
	collision_shape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, collision_shape, inertia);
	btRigidBody* rigid_body = new btRigidBody(rigid_body_ci);
	bullet.dynamics_world->addRigidBody(rigid_body);
	level_bodies.push_back(rigid_body);
	level_shapes.push_back(collision_shape);
	x = stop_x;
      }
      else {
	x++;
      }
    }
  }
}
