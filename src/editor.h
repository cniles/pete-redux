#ifndef EDITOR_H_ 
#define EDITOR_H_

#include <map>

struct MousePosition {
  int x, y;
  int dx, dy;
  float world_x, world_y;
  int tile_x, tile_y;
};

struct Editor {
  const static int MAX_TILES = 10;
  const static int MAX_OBJECT = 99;
  const static int TILE = 1;
  const static int OBJECT = 2;

  static std::map<int, GLuint> token_textures;

  float camera_x, camera_y;

  int tile_type, object_type;

  int mode;  
 
  bool dragging;

  MousePosition mouse;

  Level level;

  Editor(Level);
  
  void update();
  void draw();
  void drawTokens() const;
  void handleEvent(const SDL_Event& event);
  void flipTokens();
  static void loadStaticAssets();
  inline void setLevel(const Level& lvl) { level = lvl; }
  inline std::string getLevelName() const { return level.getFileName(); }
};

#endif
