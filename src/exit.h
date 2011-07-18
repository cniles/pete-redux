#ifndef EXIT_H_
#define EXIT_H_

#include "physicsobject.h"

class Exit : public PhysicsObject {
  static CollisionScheme exit_collision_scheme;
 
  static Animation animation;

  int rotation;
 public:
  Exit(GameState*, btVector3 position);
  ~Exit();
  static void loadStaticAssets();
  void update(float dt);
};

#endif
