#ifndef DRAW_H_
#define DRAW_H_

#include <SDL/SDL_opengl.h>

inline void drawQuad() {
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1); glVertex2f(0,0);
  glTexCoord2i(1, 1); glVertex2f(1,0);
  glTexCoord2i(1, 0); glVertex2f(1,1);
  glTexCoord2i(0, 0); glVertex2f(0,1);
  glEnd();
}

inline void drawQuadFlip() {
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1); glVertex2f(1,0);
  glTexCoord2i(1, 1); glVertex2f(0,0);
  glTexCoord2i(1, 0); glVertex2f(0,1);
  glTexCoord2i(0, 0); glVertex2f(1,1);
  glEnd();
}

inline void drawCube() {
  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.5f);
  drawQuad(); 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, 0.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  drawQuadFlip();
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(1.0f, 0.0f, 0.5f);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  drawQuad();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0f, 1.0f, 0.5f);
  glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  drawQuad();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -0.5f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
  drawQuad();
  glPopMatrix();
}

#endif
