#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "tileset.h"

struct GameObjectToken {
  int x, y;
  int id;
  GameObjectToken();
  GameObjectToken(int x, int y, int id = 0);
  bool operator==(const GameObjectToken& right);
  bool operator<(const GameObjectToken& right);
  bool operator>(const GameObjectToken& right);
  bool operator==(const int right);
  bool operator<(const int right);
  bool operator>(const int right);
};

class Level;
struct TileDrawPlan {
  TileDrawPlan();
  static TileDrawPlan createPlan(int x, int y, const Level& level);
  bool draw_top, draw_bottom, draw_left, draw_right;
};

typedef std::vector<GameObjectToken> TokenList;
typedef std::vector<GameObjectToken>::iterator TokenIter;
typedef std::vector<GameObjectToken>::const_iterator ConstTokenIter;

class Level {
  std::string file_name;
  int height;
  int length;
  Tileset front_tileset;
  Tileset top_tileset;
  std::vector<std::vector<TileDrawPlan> > plans;
  std::vector<std::vector<int> > tiles;
  std::vector<GameObjectToken> objects;
public:
  Level(int length, int height, std::string file_name);
  inline std::string getFileName() const { return file_name; };
  inline int getLength() const { return length; }
  inline int getHeight() const { return height; }
  inline int getTile(int x, int y) const { return (x>=0 && x < tiles.size() && y >=0 && y < 10 )?tiles[x][y] : -1;}
  inline int setTile(int x, int y, int id) { tiles[x][y] = id; }

  TokenIter getObjectAt(int, int);

  inline int addObject(GameObjectToken object) { objects.push_back(object); }
  GameObjectToken getObject(int id);
  
  void removeObject(TokenIter);

  inline ConstTokenIter getTokensStart() const {
    return objects.begin();
  }
  inline ConstTokenIter getTokensEnd() const {
    return objects.end();
  }

  inline TokenIter getTokensStart() {
    return objects.begin();
  }
  inline TokenIter getTokensEnd() {
    return objects.end();
  }
  void createPlans();
  void drawTile(int, int) const;
  void draw(int,int) const;
};

Level loadLevel(const char* level_file);
void saveLevel(const Level& level);

#endif
