#include "animation.h"
#include <fstream>
#include <iostream>
#include <SDL/SDL_image.h>

#include "texture.h"

Clip::Clip(int frame_count, int frame_duration) 
  : frame_count(frame_count), frame_duration(frame_duration) {
}

Animation::Animation() {
}

/*void printSurfaceFlags(SDL_Surface* surface) {
  const char* yes = "YES";
  const char* no = "NO";
  const char* srcalpha_on = (surface->flags & SDL_SRCALPHA)? yes : no;
  const char* amask_defined = (surface->format->Amask != 0)? yes : no;
  const char* srccolorkey_on = (surface->flags & SDL_SRCCOLORKEY)? yes : no;
  int bpp = surface->format->BitsPerPixel;
  std::cerr << "SDL_SRCALPHA: " << srcalpha_on << ", Amask def: " << amask_defined << ", SDL_SRCCOLORKEY: " << srccolorkey_on << ", BPP: " << bpp << std::endl;
  }*/

void Animation::parseClip(int index, Clip& clip, SDL_Surface* source) {
  SDL_Surface* sub_image = SDL_CreateRGBSurface(0, frame_size, frame_size, 32, RMASK, GMASK, BMASK, AMASK);
  int x = index * frame_size;

  SDL_SetAlpha(source, 0, 0);
  SDL_SetAlpha(sub_image, 0, 0);

  /*std::cerr << "src--";
  printSurfaceFlags(source);
  std::cerr << "dst--";
  printSurfaceFlags(sub_image);*/

  SDL_Rect src_rect = {x*frame_size, 0, frame_size, frame_size};
  SDL_Rect dst_rect = {0, 0, frame_size, frame_size };
  for(int i = 0; i < clip.frame_count; i++) {
    SDL_BlitSurface(source, &src_rect, sub_image, &dst_rect);
    src_rect.y += frame_size;
    clip.frames.push_back(getGLTexture(sub_image));
  }
  SDL_FreeSurface(sub_image);
}

Animation::Animation(const char* fname) {
  std::fstream ifile(fname, std::fstream::in);
  char image_fname[80];
  ifile.getline(image_fname,80);
  ifile >> frame_size;
  int frame_count;
  int frame_duration;
  while((ifile >> frame_count).good()) {
    ifile >> frame_duration;
    clips.push_back(Clip(frame_count, frame_duration));
  }

  SDL_Surface* surface = IMG_Load(image_fname);
  for(int i = 0; i < clips.size(); i++) {
    parseClip(i, clips[i], surface);
  }
  SDL_FreeSurface(surface);
  ifile.close();
}

void AnimationTimer::advanceFrame() {
  current_frame++;
  if(current_frame >= animation->getFrameCount(current_clip)) {
    current_frame = 0;
  }
}

void AnimationTimer::tick(Uint32 dt) {
  if(status == PLAYING) {
    timer-= dt;
    if(timer <= 0) {
      advanceFrame();
      timer += animation->getDuration(current_clip);
    }
  }
}

void AnimationTimer::setClip(int id) {
}

void AnimationTimer::start() {
  timer = animation->getDuration(current_clip);
  status = PLAYING;
}

AnimationTimer::AnimationTimer() 
  : current_clip(0), current_frame(0), timer(0), status(0), animation(NULL) {
}

AnimationTimer::AnimationTimer(Animation* animation)
  : current_clip(0), current_frame(0), timer(0), status(0), animation(animation) {

}
