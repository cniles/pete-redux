#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define BPP 32

#define AMASK 0xff000000
#define BMASK 0x00ff0000
#define GMASK 0x0000ff00
#define RMASK 0x000000ff

void setDefaultTexParameter();
GLuint getGLTexture(SDL_Surface*);

#endif
