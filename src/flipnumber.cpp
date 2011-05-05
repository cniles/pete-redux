#include "flipnumber.h"
#include "draw.h"

Animation FlipNumber::animation;

FlipNumber::FlipNumber(float x, float y, int value, int digits)
  : x(x), y(y), value(value), digits(digits) {
}

void FlipNumber::draw() {
  int temp = value;
  int numbers[digits];
  glTranslatef(x, y, 0.0f);
  for(int i = 1; i <= digits; i++) {
    numbers[digits - i] = temp % 10;
    temp /= 10;
  }
  for(int i = 0; i < digits; i++) {
    glTranslatef(24.0f, 0.0f, 0.0f);
    glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, numbers[i]));
    drawQuad();
  }
}

void FlipNumber::loadStaticAssets() {
  animation = Animation("gfx/numbers");
}
