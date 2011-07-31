#include "statemachineobject.h"
#include "gamestate.h"

State::State(StateMachineObject* owner)
  : __owner(owner) {
}

StateMachineObject::StateMachineObject(GameState* gamestate, const btVector3& position, Animation* animation, CollisionScheme scheme, State* state) 
  : PhysicsObject(gamestate, position, animation, scheme), current_state(NULL), disable_update(false) {
  changeState(state);
}

StateMachineObject::~StateMachineObject() {
  if(current_state) delete current_state;
}

btVector3 StateMachineObject::getFurthestFacingPointOnPlatform() {
  Level* level = &(gamestate->level);
  int origin_x = position.getX();
  int origin_y = position.getY();
  bool position_found = false;
  int x = origin_x;
  while(!position_found) {
    int search_x = x + direction;
    if(level->getTile(search_x,origin_y) == 0 && level->getTile(search_x, origin_y - 1) != 0) {
      x = search_x;
    }
    else {
      position_found = true;
    }
  }
  return btVector3(x + 0.5f, origin_y + 0.5f, 0);
}

bool StateMachineObject::atPlatformEnd() const {
  Level* level = &(gamestate->level);
  float origin_x = position.getX();
  float origin_y = position.getY(); 
  float stop_x = (int)origin_x + 0.5f;
  float stop_y = (int)origin_y + 0.5f;
  int next_x = origin_x + direction;
  if(level->getTile(next_x, origin_y -1) == 0 &&
     origin_x - stop_x < 0.01f &&
     origin_y - stop_y < 0.01f) {
    return true;
  }
  return false;
}

void StateMachineObject::changeState(State* new_state) {
  State* temp = current_state;
  if(current_state) {
    current_state->onLeave();
  }
  current_state = new_state;
  if(current_state) {
    current_state->onEnter();
  }
  if(temp) {
    delete temp;
  }
}

void StateMachineObject::update(float dt) {
  animation_timer.tick(dt*1000.0f);
  if(current_state != NULL && !disable_update) {
    current_state->onUpdate(dt);
  }
}

void StateMachineObject::stopUpdate() {
  disable_update = true;
}

float StateMachineObject::getDistance2ToPlayer() {
  return position.distance2(gamestate->player->getPosition());
}

bool StateMachineObject::lookForPlayer(const btVector3& direction) {
  btVector3 new_direction = direction + position;
  btCollisionWorld::ClosestRayResultCallback callback(position, new_direction);
  callback.m_collisionFilterGroup = COL_ENEMY;
  callback.m_collisionFilterMask = COL_PLAYER | COL_LEVEL; // can see through other enemies
  gamestate->dynamics_world->rayTest(position, new_direction, callback);
  if(callback.hasHit()) {
    if(callback.m_collisionObject) {
       if(callback.m_collisionObject->getUserPointer() == gamestate->player) {	
	return true;
      }
    }
  }  
  return false;
}

bool StateMachineObject::isWalkableTile(int x, int y, const Level& level) {
  if(level.getTile(x,y)==0 && level.getTile(x, y-1)==0) {
    return true; 
  }
  return false;
}
