#include <iostream>
#include <fstream>
#include <algorithm>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#define TILE_SIZE 1

#include "tileset.h"
#include "level.h"
#include "draw.h"

GameObjectToken::GameObjectToken()
  : x(0), y(0), id(0) {
}

GameObjectToken::GameObjectToken(int x, int y, int id)
  : x(x), y(y), id(id) {
}

bool GameObjectToken::operator==(const GameObjectToken& right) {
  return id==right.id;
}

bool GameObjectToken::operator<(const GameObjectToken& right) {
  return id<right.id;
}

bool GameObjectToken::operator>(const GameObjectToken& right) {
  return id>right.id;
}

bool GameObjectToken::operator==(const int right) {
  return id==right;
}

bool GameObjectToken::operator<(const int right) {
  return id<right;
}

bool GameObjectToken::operator>(const int right) {
  return id>right;
}

TileDrawPlan::TileDrawPlan()
  : draw_right(false), draw_left(false), draw_top(false), draw_bottom(false) {
}

TileDrawPlan TileDrawPlan::createPlan(int x, int y, const Level& level) {
  TileDrawPlan plan;
  if(level.getTile(x+1, y) ==0) {
    plan.draw_right = true;
  }
  if(level.getTile(x-1, y) ==0) {
    plan.draw_left = true;
  }
  if(level.getTile(x, y+1) ==0) {
    plan.draw_top = true;
  }
  if(level.getTile(x, y-1) ==0) {
    plan.draw_bottom = true;
  }
  return plan;
  
}

Level::Level(int length, int height, std::string file_name)
  : length(length), height(height), file_name(file_name) {
  plans = std::vector<std::vector<TileDrawPlan> >(length, std::vector<TileDrawPlan>(height));
  tiles = std::vector<std::vector<int> >(length, std::vector<int>(height));
  front_tileset = loadTileset("gfx/tiles.bmp");
  top_tileset = loadTileset("gfx/toptiles.bmp");
}

void Level::drawTile(int x, int y) const {
  int tile = tiles[x][y];

  glPushMatrix();
  glTranslatef(x, y, 0);
  
  int texture = front_tileset[tile].getTexture();
  glBindTexture(GL_TEXTURE_2D, texture);
  
  drawQuad(); //face
  
  if(plans[x][y].draw_right) {
    glPushMatrix();
    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawQuad();
    glPopMatrix();
  }

  if(plans[x][y].draw_left) {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    drawQuad();
    glPopMatrix();
  }

  if(plans[x][y].draw_top) {
    texture = top_tileset[tile].getTexture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawQuad();
    glPopMatrix();
  }

  if(plans[x][y].draw_bottom) {
    texture = top_tileset[tile].getTexture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawQuad();
    glPopMatrix();
  }

  glPopMatrix();
}

void Level::draw(int start_col, int end_col) const {
  using std::max;
  using std::min;
  start_col = min(max(0, length), length);
  end_col = min(max(0, end_col), length);
  for(int i = 0; i < length; i++) {
    for(int j = 0; j < height; j++) {
      if(tiles[i][j]!=0)
	drawTile(i, j);
    }
  }
}

GameObjectToken Level::getObject(int id) {
  std::vector<GameObjectToken>::iterator token_iter = std::find(objects.begin(), objects.end(), id);
  return (*token_iter);
}

TokenIter Level::getObjectAt(int x, int y) {
  TokenIter token_iter = objects.begin();
  while(token_iter != objects.end()){
    if(token_iter->x == x && token_iter->y == y) {
      return token_iter;
    }
    token_iter++;
  }
  return objects.end();
}

void Level::removeObject(TokenIter iter) {
  objects.erase(iter);
}

void Level::createPlans() {
  for(int x = 0; x < length; x++) {
    for(int y = 0; y < height; y++) {
      plans[x][y] = TileDrawPlan::createPlan(x, y, *this);
    }
  }
}

Level loadLevel(const char* level_file) {
  std::fstream file(level_file, std::fstream::in);
  int id_in, x, y;
  int length;
  int height;
  file >> length;
  file >> height;
  Level result(length, height, level_file);
  for(int j = 9; j >= 0; j--) {
    for(int i = 0; i < length; i++) {
      file >> id_in;
      result.setTile(i, j, id_in);
    }
  }
  while((file >> id_in).good()) {
    file >> x >> y;
    result.addObject(GameObjectToken(x, y, id_in));
  }
  result.createPlans();
  return result;
}

void saveLevel(const Level& level) {
  std::fstream file(level.getFileName().c_str(), std::fstream::out);
  int length = level.getLength();
  int height = level.getHeight();
  file << length << ' ' << height << '\n';
  for(int y = 1; y <= height; y++) {
    for(int x = 0; x < length; x++) {
      file << level.getTile(x, height-y) << ' ';
    }
    file << '\n';
  }
  ConstTokenIter token_iter = level.getTokensStart();
  while(token_iter != level.getTokensEnd()) {
    file << token_iter->id << ' ' << token_iter->x << ' ' << token_iter->y << '\n';
    token_iter++; 
  }
}
