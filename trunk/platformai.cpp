#include "platformai.h"
#include <iostream>
#include <sstream>
#include <cmath>

AIVar::AIVar(float f) {
  f_v = f;
}

AIVar::AIVar(int i = 0) {
  i_v = i;
}

State::State(PlatformAI* ai) 
  : ai(ai) {
}

PlatformAI::~PlatformAI() {
  if(current_state != NULL) {
    delete current_state;
  }
}

PlatformAI::PlatformAI() 
  : gamestate(NULL), owner(NULL), current_state(NULL) {
}

PlatformAI::PlatformAI(GameState* gamestate, GameObject* owner) 
  : gamestate(gamestate), owner(owner), current_state(NULL) {
}

void PlatformAI::changeState(State* new_state) {
  State* temp = current_state;
  if(current_state != NULL) {
    current_state->onLeave();
  }
  current_state = new_state;
  if(current_state != NULL) {
    current_state->onEnter();
  }
  if(temp != NULL) {
    delete temp;
  }
}

void PlatformAI::update(float dt) {
  if(current_state != NULL) {
    current_state->onUpdate(dt);
  }
}

AIVar PlatformAI::getVar(string name) {
  VarBank::iterator var_iter = var_bank.find(name);
  if(var_iter == var_bank.end()) {
    std::stringstream sstr;
    sstr << "Could not find AI variable " << name;
    throw sstr.str();
  }
  return var_iter->second;
}

void PlatformAI::setVar(string name, AIVar var) {
  var_bank[name] = var;
}

btVector3 PlatformAI::getRandomDestinationOnPlatform(float range) {
  Level* level = &(gamestate->level);
  
  float platform_width;
}

btVector3 PlatformAI::getFurthestFacingPointOnPlatform() {
  Level* level = &(gamestate->level);
  int origin_x = owner->getPosition().getX();
  int origin_y = owner->getPosition().getY();
  int direction = owner->getDirection();
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
  return btVector3(x, origin_y, 0);
}

void PlatformAI::setDestination(btVector3 destination) {
  this->destination = destination;
}
