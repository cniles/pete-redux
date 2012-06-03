#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <iostream>

#define BPP 32

#include "texture.h"

void setDefaultTexParameter() {
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
}

SDL_Surface* createConvertedSurface(SDL_Surface* original) {
  int w = original->w;
  int h = original->h;
  SDL_Surface* result = SDL_CreateRGBSurface(SDL_SRCALPHA, w, h, BPP, RMASK, GMASK, BMASK, AMASK);

  SDL_SetAlpha(original, 0, 0);
  //SDL_SetAlpha(result, 0, 0);

  SDL_BlitSurface(original, NULL, result, NULL);
  return result;
}

GLuint buildGLTexture(SDL_Surface* surface) {
  GLuint result = 0;
  glGenTextures(1, &result);
  glBindTexture(GL_TEXTURE_2D, result);
  setDefaultTexParameter();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  return result;
}

GLuint getGLTexture(SDL_Surface* loaded_image) {
  GLuint result = 0;
  if(loaded_image) {
    SDL_Surface* converted_image = createConvertedSurface(loaded_image);
    result = buildGLTexture(converted_image);
    SDL_FreeSurface(converted_image);
  }
  return result;
}
