#ifndef MEDPACK_H_
#define MEDPACK_H_

#include "physicsobject.h"

class Medpack : public PhysicsObject {
  const static int HEALTH_REWARD = 8;

  static Animation animation;

  int rotation;
 public:
  Medpack(GameState*, btVector3 position);
  ~Medpack();
  static void loadStaticAssets();
  void update(float dt);
};

#endif
