#ifndef AMMO_H_
#define AMMO_H_

#include "physicsobject.h"

class Ammo : public PhysicsObject {
  const static int AMMO_REWARD = 8;
  const static int rotation_speed = 180; // degrees/sec
  int rotation;
  static Animation animation;

 public:
  Ammo(GameState*, btVector3 position);
  static void loadStaticAssets();
  void update(float dt);
  void draw();
};

#endif
