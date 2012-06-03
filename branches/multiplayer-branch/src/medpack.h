#ifndef MEDPACK_H_
#define MEDPACK_H_

#include "physicsobject.h"

class Medpack : public PhysicsObject {
  const static int HEALTH_REWARD = 8;
  const static float MASS;
  static Animation animation;

  static CollisionScheme collision_scheme;

  int rotation;
 public:
  Medpack(GameState*, const btVector3& position);
  ~Medpack();
  static void loadStaticAssets();
  void update(float dt);
};

#endif
