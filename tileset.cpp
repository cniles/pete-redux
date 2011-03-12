#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>

#define BPP 32

#include "tileset.h"
#include "texture.h"

Tile::Tile(int id, GLuint texture)
  : id(id), texture(texture) {
}

Tileset loadTileset(const char* fname) {
  SDL_Surface* loaded_image = NULL;
  Tileset result;
  loaded_image = IMG_Load(fname);
  if(loaded_image) {
    int size = loaded_image->h;
    SDL_Surface* sub_image = SDL_CreateRGBSurface(0, size, size, BPP, RMASK, GMASK, BMASK, AMASK);
    SDL_Rect source_rect = {0,0,size,size};
    SDL_Rect destination_rect = {0,0,size,size};
    GLuint new_texture;
    for(int read_position = 0; read_position < (loaded_image->w/size); read_position++) {
      SDL_BlitSurface(loaded_image, &source_rect, sub_image, &destination_rect);
      new_texture = getGLTexture(sub_image);
      result.push_back(Tile(read_position, new_texture));
      source_rect.x += size;
    }
    SDL_FreeSurface(loaded_image);
    SDL_FreeSurface(sub_image);
  }
  return result;
}
