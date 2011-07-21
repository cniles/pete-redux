#ifndef AMMO_H_
#define AMMO_H_

#include "physicsobject.h"

class Ammo : public PhysicsObject {
  const static int AMMO_REWARD = 8;
  const static int rotation_speed = 180; // degrees/sec

  static Animation animation;

  int rotation;
 public:
  Ammo(GameState*, const btVector3& position);
  ~Ammo();
  static void loadStaticAssets();
  void update(float dt);
  void draw();
};

#endif
