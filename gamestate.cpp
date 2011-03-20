#include "zombie.h"
#include "gamestate.h"
#include <iostream>

#include <BulletCollision/CollisionShapes/btBox2dShape.h>

GameState::GameState(Level level)
  : level(level) {
  std::cerr << "Initializing dynamics world...";
  bullet_core = createPhysicsWorld();
  dynamics_world = bullet_core->dynamics_world;
  std::cerr << "Successfully created dynamics world." << std::endl;

  std::cerr << "Initializing player...";
  GameObjectToken player_position = level.getObject(30);
  player = new Player(player_position.x, player_position.y, this);
  std::cerr << "Successfully initialzed player." << std::endl;

  std::cerr << "Initializing game objects...";
  TokenList::const_iterator token_iter = level.getTokensStart();
  while(token_iter != level.getTokensEnd()) {
    switch(token_iter->id) {
    case 0:
      DEBUG_OUT("New Zombie");
      objects.push_back(new Zombie(this, btVector3(token_iter->x + 0.5f, token_iter->y + 0.5f, 0.0f)));
      break;
    }
    token_iter++;
  }
  std::cerr << "Successfully initialized game objects." << std::endl;

  std::cerr << "Initializing level physics...";
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
	btCollisionShape* collision_shape = new btBox2dShape(btVector3(width/2.0f,0.5f,0.5f));
	btDefaultMotionState* motion_state = 
	  new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(center_x, y+0.5f, 0)));
	btScalar mass = 0;
	btVector3 inertia(0,0,0);
	collision_shape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(mass, motion_state, collision_shape, inertia);
	btRigidBody* rigid_body = new btRigidBody(rigid_body_ci);
	rigid_body->setUserPointer(NULL);
	dynamics_world->addRigidBody(rigid_body, COL_LEVEL, COL_ENEMY | COL_PLAYER);
	level_bodies.push_back(rigid_body);
	level_shapes.push_back(collision_shape);
	x = stop_x;
      }
      else {
	x++;
      }
    }
  }
  std::cerr << "Successfully initializinged level physics." << std::endl;
}

GameState::~GameState() {
  delete player;
  for(int i = 0; i < objects.size(); i++) {
    delete objects[i];
  }
  for(int i = 0; i < level_bodies.size(); i++) {
    delete level_bodies[i]->getMotionState();
    dynamics_world->removeRigidBody(level_bodies[i]);
    delete level_bodies[i];
    delete level_shapes[i];
  }
  delete bullet_core;
}
