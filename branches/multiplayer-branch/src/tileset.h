#ifndef TILESET_H
#define TILESET_H

#include <SDL/SDL_opengl.h>
#include <vector>

class Tile {
  int id;
  GLuint texture;
public:
  Tile();
  Tile(int, GLuint);
  GLuint getTexture() const { return texture; }
};

typedef std::vector<Tile> Tileset;

Tileset loadTileset(const char* fname);

#endif
