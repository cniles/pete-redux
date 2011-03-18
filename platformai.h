#ifndef PLATFORMAI_H_
#define PLATFORMAI_H_

#include <map>
#include <string>

#include "gamestate.h"
#include "gameobject.h"

using std::string;

// quick state-adding macro for code brevity.
#define MAKE_STATE( STATE_NAME ) class STATE_NAME : public State { \
  public:							   \
  STATE_NAME(PlatformAI* ai) : State(ai) {}			   \
    void onEnter();						   \
    void onUpdate(float);					   \
    void onLeave();						   \
  };


struct AIVar {
  union {
    float f_v;
    int i_v;
    struct {
      short si1_v;
      short si2_v;
    };
  };
  AIVar(float f);
  AIVar(int i);
};

typedef std::map<string, AIVar> VarBank;

class PlatformAI;
class State {
 protected:
  PlatformAI* ai;
 public:
  State(PlatformAI* ai);
  virtual void onEnter() = 0;
  virtual void onUpdate(float dt) = 0;
  virtual void onLeave() = 0;
};

class PlatformAI {
  GameState* gamestate;
  GameObject* owner;
  VarBank var_bank;
  State* current_state;

  btVector3 destination;
 public:
  PlatformAI();
  PlatformAI(GameState*, GameObject*);
  ~PlatformAI();
  void update(float dt);
  void changeState(State* new_state);
  AIVar getVar(string name);
  void setVar(string name, AIVar var);
  inline GameObject* getOwner() { return owner; }
  btVector3 getRandomDestinationOnPlatform(float range = -1);
  btVector3 getFurthestFacingPointOnPlatform();
  void setDestination(btVector3 destination);
  inline btVector3 getDestination() { return destination; }
};

#endif
  
