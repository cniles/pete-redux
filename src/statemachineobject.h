#ifndef STATEMACHINEOBJECT_H_
#define STATEMACHINEOBJECT_H_

#include "physicsobject.h"

class StateMachineObject;
class State {
  void* __owner;
 public:
  State(StateMachineObject* owner);
  virtual void onEnter() = 0;
  virtual void onUpdate(float dt) = 0;
  virtual void onLeave() = 0;
};

// quick state-adding macro for code brevity.
#define MAKE_STATE( STATE_NAME, OWNER_CLASS )				\
  class STATE_NAME : public State {					\
    OWNER_CLASS* owner;							\
  public:								\
  STATE_NAME(StateMachineObject* owner) : State(owner),			\
      owner((OWNER_CLASS*)owner) {}					\
    void onEnter();							\
    void onUpdate(float);						\
    void onLeave();							\
  };
  
class StateMachineObject : public PhysicsObject {
 private:
  State* current_state;
  bool disable_update;
 protected:
  StateMachineObject(GameState* gamestate, const btVector3& position, Animation* animation, CollisionScheme scheme, State* state);
  ~StateMachineObject();
  void stopUpdate();
  void update(float dt);
 public:
  void changeState(State* new_state);
  btVector3 getFurthestFacingPointOnPlatform();
  float getDistance2ToPlayer();
  bool atPlatformEnd() const;
  bool lookForPlayer(const btVector3& direction);
};

#endif
 
