#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define BPP 32

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define RMASK 0xff000000
    #define GMASK 0x00ff0000
    #define BMASK 0x0000ff00
    #define AMASK 0x000000ff
#else
    #define RMASK 0x000000ff
    #define GMASK 0x0000ff00
    #define BMASK 0x00ff0000
    #define AMASK 0xff000000
#endif

void setDefaultTexParameter();
GLuint getGLTexture(SDL_Surface*);

#endif
