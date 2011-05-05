#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>

#include "bullet.h"
#include "tileset.h"
#include "keystates.h"
#include "gamestate.h"
#include "texture.h"
#include "draw.h"

#include "objecttypes.h"

int screen_width = 640;
int screen_height = 480;
int screen_bpp = 32;

const char* LEVEL_FILE_NAME = "levels/aitest.txt";
const char* BACKGROUND_IMAGE_FILE_NAME = "gfx/background.png";
GLuint background;

KeyStates::KeyStates() {
  memset(this, 0x0, sizeof(KeyStates));
}

GLuint loadTexture(const char* file_name) {
  SDL_Surface* surface = IMG_Load(file_name);
  GLuint texture = getGLTexture(surface);
  SDL_FreeSurface(surface);
  return texture;
}

void loadStaticAssets() {
  Zombie::loadStaticAssets();
  Bat::loadStaticAssets();
  Soul::loadStaticAssets();
  DarkChampion::loadStaticAssets();
  Ammo::loadStaticAssets();
  Medpack::loadStaticAssets();
}

void initSDL() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_WM_SetCaption( "Pete Re-dux", NULL );
  SDL_SetVideoMode(screen_width, screen_height, screen_bpp, SDL_OPENGL);
}

void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)screen_width/(float)screen_height, 1, 30);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glCullFace(GL_BACK);
}

void handleKeys(SDL_Event& event, KeyStates& key_states) {
  bool set_value = (event.type == SDL_KEYDOWN)? true : false;
  if(event.key.keysym.sym == SDLK_LCTRL) {
    key_states.ctrl_held = set_value;
  }
  if(event.key.keysym.sym == SDLK_UP) {
    key_states.up_held = set_value;
  }
  if(event.key.keysym.sym == SDLK_DOWN) {
    key_states.down_held = set_value;
  }
  if(event.key.keysym.sym == SDLK_LEFT) {
    key_states.left_held = set_value;
  }
  if(event.key.keysym.sym == SDLK_RIGHT) {
    key_states.right_held = set_value;
  }
  if(event.key.keysym.sym == SDLK_SPACE) {
    key_states.space_held = set_value;
  }
}

inline void drawBackground() {
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glTranslatef(0.0f, 0.0f, -3.0f);
  glScalef(4.0f, 4.0f, 1.0f);
  glScalef(1.33f, 1.0f, 1.0f); // Aspect ratio of background image.
  glColor4f(0.5f, 0.5f, 0.5f,1.0f);
  glTranslatef(-0.5f, -0.5f, 0.0f);
  glBindTexture(GL_TEXTURE_2D, background);
  drawQuad();
  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
}

void draw(const GameState& gamestate) {  
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  drawBackground();

  glPushMatrix();   
  glTranslatef(-gamestate.player->getX(), -gamestate.player->getY(), -10.0f);

  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  gamestate.level.draw(0,12); 

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glAlphaFunc(GL_GREATER, 0.1f);
  glEnable(GL_ALPHA_TEST);

  gamestate.player->draw(); 
  std::vector<GameObject*>::const_iterator object_iter = gamestate.objects.begin();
  while(object_iter != gamestate.objects.end()) {
      (*object_iter)->draw();
      object_iter++;
  }  
  glPopMatrix();
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  SDL_GL_SwapBuffers();
}

void processCollisions(GameState* gamestate) {
  int num_manifolds = gamestate->dynamics_world->getDispatcher()->getNumManifolds();

  for(int i = 0; i < num_manifolds; i++) {
    btPersistentManifold* contact_manifold = gamestate->dynamics_world->getDispatcher()->getManifoldByIndexInternal(i);

    btCollisionObject* object_a = static_cast<btCollisionObject*>(contact_manifold->getBody0());
    btCollisionObject* object_b = static_cast<btCollisionObject*>(contact_manifold->getBody1());

    void* pointer_a = object_a->getUserPointer();
    void* pointer_b = object_b->getUserPointer();

    if(pointer_a == NULL || pointer_b == NULL) continue;

    if(pointer_a == gamestate->player || pointer_b == gamestate->player) {     
      PhysicsObject* other = (PhysicsObject*)((pointer_a == gamestate->player) ? pointer_b : pointer_a);
      other->notifyCollisionWithPlayer();
      //gamestate->player->notifyCollisionWithObject(other);
    }

    /*int num_contacts = contact_manifold->getNumContacts();
    for(int j = 0; j < num_contacts; j++) {
      btManifoldPoint& point = contact_manifold->getContactPoint(j);
      }*/
  }
}

Uint32 start = 0;
Uint32 stop = 0;
void run() {
  float dtf = 0.0f;
  Uint32 elapsed = 0;
  SDL_Event event;
  bool quit = false;

  srand(time(NULL));

  GameState gamestate(loadLevel(LEVEL_FILE_NAME));

  KeyStates key_states;
  
  while(!quit) {
    stop = SDL_GetTicks();
    if(start != 0) {
      elapsed = stop - start;
    }
    start = stop;
    dtf = elapsed / 1000.0f;

    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
	quit = true;
      }
      if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
	handleKeys(event, key_states);
	if(event.key.keysym.sym == SDLK_ESCAPE) {
	  quit = true;
	}
      }
    }

    gamestate.dynamics_world->stepSimulation(dtf,10);
    processCollisions(&gamestate);

    gamestate.player->update(elapsed, key_states);

    std::vector<GameObject*>::const_iterator object_iter = gamestate.objects.begin();
    while(object_iter != gamestate.objects.end()) {
      (*object_iter)->update(dtf);
      object_iter++;
    }

    draw(gamestate);
  }
}

int main() {
  initSDL();
  initGL();
  loadStaticAssets();
  background = loadTexture(BACKGROUND_IMAGE_FILE_NAME);
  run();
  SDL_Quit();
  return 0;
}
