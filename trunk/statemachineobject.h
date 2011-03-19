#ifndef STATEMACHINEOBJECT_H_
#define STATEMACHINEOBJECT_H_

#include "physicsobject.h"

class StateMachineObject;
class State {
 protected:
  StateMachineObject* owner;
 public:
  State(StateMachineObject* owner);
  virtual void onEnter() = 0;
  virtual void onUpdate(float dt) = 0;
  virtual void onLeave() = 0;
};

// quick state-adding macro for code brevity.
#define MAKE_STATE( STATE_NAME ) class STATE_NAME : public State { \
  public:							   \
  STATE_NAME(StateMachineObject* owner) : State(owner) {}			   \
    void onEnter();						   \
    void onUpdate(float);					   \
    void onLeave();						   \
  };

class StateMachineObject : public PhysicsObject {
 private:
  State* current_state;
  btVector3 destination;

 protected:
  StateMachineObject(GameState* gamestate, btVector3 position, Animation* animation, CollisionScheme scheme, State* state);
  void update(float dt);
 public:
  void changeState(State* new_state);
  btVector3 getFurthestFacingPointOnPlatform();
  inline btVector3 getDestination() { return destination; }
  inline void setDestination(btVector3 destination) { this->destination = destination; }
};

#endif
