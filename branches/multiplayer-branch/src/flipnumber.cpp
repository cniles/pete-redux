#include "flipnumber.h"
#include "draw.h"

Animation FlipNumber::animation = Animation();

FlipNumber::FlipNumber(float x, float y, unsigned int value, int digits)
  : x(x), y(y), value(value), digits(digits) {
}

void FlipNumber::draw() {
  unsigned int temp = value;
  int* numbers = new int[digits];


  for(int i = 1; i <= digits; i++) {
    numbers[digits - i] = temp % 10;
    temp /= 10;
  }

  for(int i = 0; i < digits; i++) {
    glBindTexture(GL_TEXTURE_2D, animation.getFrame(0, numbers[i]));
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glTranslatef(24.0f * i, 0.0f, 0.0f);
    glScalef(24.0f, 24.0f, 0.0f);
    drawQuad();
    glPopMatrix();
  }
  delete[](numbers);
}

void FlipNumber::loadStaticAssets() {
  animation = Animation("gfx/numbers");
}
