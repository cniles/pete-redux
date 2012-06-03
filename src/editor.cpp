#include <fstream>
#include <string>
#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "level.h"
#include "editor.h"
#include "draw.h"
#include "flipnumber.h"
#include "texture.h"
void updateMouseWorldCoordinates(MousePosition& mouse);

std::map<int, GLuint> Editor::token_textures;

Editor::Editor(Level level)
  : camera_x(0), camera_y(0), tile_type(1), object_type(0), dragging(false), level(level), mode(TILE), hide_level(false) {
}

void Editor::update() {
  if(dragging) {
    camera_x += mouse.dx / 10.0f;
    camera_y -= mouse.dy / 10.0f;
  }
  mouse.dx = 0.0f;
  mouse.dy = 0.0f;
}

void Editor::handleEvent(const SDL_Event& event) {
  switch(event.type) {
  case(SDL_KEYDOWN):
    switch(event.key.keysym.sym) {
    case(SDLK_h):
      hide_level = true;
      break;
    }
    break;
  case(SDL_KEYUP):
    switch(event.key.keysym.sym) {
    case(SDLK_LEFTBRACKET):
      if(mode == TILE) {
	(--tile_type) = (tile_type < 0) ? MAX_TILES-1 : tile_type;
      }
      if(mode == OBJECT) {
	(--object_type) = (object_type < 0) ? MAX_OBJECT: object_type;
      }
      break;
    case(SDLK_RIGHTBRACKET):
      if(mode == TILE) {
	(++tile_type) = (tile_type > MAX_TILES) ? 0 : tile_type;
      }
      if(mode == OBJECT) {
	(++object_type) = (object_type > MAX_OBJECT) ? 0 : object_type;
      }
      break;
    case(SDLK_BACKSLASH):
      mode ^= (TILE | OBJECT);
      break;
    case(SDLK_F6):
      std::cerr << "Saving level..." << std::endl;
      saveLevel(level);
      break;
    case(SDLK_F9): 
      flipTokens();
      break;
    case(SDLK_h):
      hide_level = false;
      break;
    };
    break;
  case(SDL_MOUSEMOTION):
    mouse.x = event.motion.x;
    mouse.y = event.motion.y;
    mouse.dx = event.motion.xrel;
    mouse.dy = event.motion.yrel;
    break;
  case(SDL_MOUSEBUTTONDOWN):
    if(event.button.button == SDL_BUTTON_MIDDLE) {
      dragging = true;
    }
    break;
  case(SDL_MOUSEBUTTONUP):
    if(event.button.button == SDL_BUTTON_MIDDLE) {
      dragging = false;
    }		
    else if(event.button.button == SDL_BUTTON_LEFT) {
      if(mouse.tile_x < 0) {
	break;
      }
      if(mode == TILE) {
	level.setTile(mouse.tile_x, mouse.tile_y, tile_type);
	level.createPlans();
      }
      else if(mode == OBJECT) {	
	TokenIter token_iter = level.getObjectAt(mouse.tile_x, mouse.tile_y);
	if(token_iter != level.getTokensEnd()) {
	  token_iter->id = object_type;
	}
	else {
	  level.addObject(GameObjectToken(mouse.tile_x, mouse.tile_y, object_type));
	}
      }
    }
    else if(event.button.button == SDL_BUTTON_RIGHT) {
      if(mouse.tile_x < 0) {
	break;
      }
      if(mode == TILE) {
	level.setTile(mouse.tile_x, mouse.tile_y, 0);
	level.createPlans();
      }
      else if(mode == OBJECT) {
	TokenIter token_iter = level.getObjectAt(mouse.tile_x, mouse.tile_y);
	if(token_iter != level.getTokensEnd()) {
	  level.removeObject(token_iter);
	}
      }
    }
    break;
  }
}

void updateMouseWorldCoordinates(MousePosition &mouse) {
  GLdouble modelview[16];
  GLdouble projection[16];
  GLint viewport[4];

  GLfloat winZ;
  GLdouble obj_x, obj_y, obj_z;

  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  //glReadPixels(mouse.x, viewport[3] - mouse.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
  winZ = 0.93f;

  gluUnProject(mouse.x, viewport[3] - mouse.y, winZ, modelview, projection, viewport, &obj_x, &obj_y, &obj_z);

  mouse.world_x = obj_x;
  mouse.world_y = obj_y;
  mouse.tile_x = (int)obj_x;
  mouse.tile_y = (int)obj_y;
}

void Editor::drawTokens() const {
  if(hide_level) {
    glDisable(GL_DEPTH_TEST);
  }
  std::vector<GameObjectToken>::const_iterator token_iter = level.getTokensStart();
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  while(token_iter != level.getTokensEnd()) {
    glBindTexture(GL_TEXTURE_2D, token_textures[token_iter->id]);
    glPushMatrix();
    glTranslatef(token_iter->x, token_iter->y, -0.5f);
    drawQuad();
    glPopMatrix();
    token_iter++;
  }
  glDisable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
}

void Editor::draw() {
  glClearColor(0.28f, 0.66f, 0.1f, 1.0f);
  GLfloat cube_points[] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, -1.0f,
    1.0f, 0.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    0.0f, 1.0f, -1.0f,
  };

  GLuint wire_cube_lines[] = {
    0, 1,
    1, 2,
    2, 3, 
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
  };

    
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  reshapePerspective();
  glLoadIdentity();

  glPushMatrix();

  glTranslatef(camera_x, camera_y, -10.0f);

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  if(hide_level) {
    glEnable(GL_BLEND);
    glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
  }
  level.draw(0,12); 
  glDisable(GL_BLEND);

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  updateMouseWorldCoordinates(mouse);

  glEnableClientState(GL_VERTEX_ARRAY);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glPointSize(8);
  glPushMatrix();
  glTranslatef(mouse.tile_x, mouse.tile_y, 0.0f);
  glVertexPointer(3, GL_FLOAT, 0, cube_points);
  glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, wire_cube_lines);
  glDrawArrays(GL_POINTS, 0, 8);
  glPopMatrix();

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glDisableClientState(GL_VERTEX_ARRAY);

  drawTokens();

  glPopMatrix();

  reshapeOrthogonal();
  
  FlipNumber mode_flip(60, 400, mode, 1);
  FlipNumber tile_x(60, 60, mouse.tile_x, 2);
  FlipNumber tile_y(120, 60, mouse.tile_y, 2);
  int m_type = (mode == TILE) ? tile_type : object_type;
  FlipNumber type(60, 30, m_type, 2);

  tile_x.draw();
  tile_y.draw();
  type.draw();
  mode_flip.draw();

  SDL_GL_SwapBuffers();  
}

void Editor::loadStaticAssets() {
  std::fstream token_file("gfx/editor/tokens", std::fstream::in);
  if(token_file.bad()) {
    std::cerr << "Error opening gfx/editor/tokens" << std::endl;
  }
  int id;
  GLuint texture;
  std::string image_name;
  while((token_file >> id >> image_name).good()) {
    SDL_Surface* image = IMG_Load(image_name.c_str());
    texture = getGLTexture(image);
    SDL_FreeSurface(image);
    token_textures.insert(std::pair<int, GLuint>(id, texture));
  }
}

void Editor::flipTokens() {
  TokenIter token_iter = level.getTokensStart();
  while(token_iter != level.getTokensEnd()) {
    token_iter->y = abs(level.getHeight() - 1 - token_iter->y);
    ++token_iter;
  }
}
