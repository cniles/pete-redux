#include "statemachineobject.h"
#include "gamestate.h"

State::State(StateMachineObject* owner)
  : __owner(owner) {
}

StateMachineObject::StateMachineObject(GameState* gamestate, btVector3 position, Animation* animation, CollisionScheme scheme, State* state) 
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
