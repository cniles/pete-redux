#ifndef FLIPNUMBER_H_
#define FLIPNUMBER_H_

#include "animation.h"

struct FlipNumber {
  int value;
  int digits;

  float x;
  float y;

  static Animation animation;

  FlipNumber(float, float, int, int);
  void draw();
  static void loadStaticAssets();
};

#endif
