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

void Animation::parseClip(int index, Clip& clip, SDL_Surface* source) {
  SDL_Surface* sub_image = SDL_CreateRGBSurface(0, frame_width, frame_height, 32, RMASK, GMASK, BMASK, AMASK);
  int x = index * frame_width;

  SDL_SetAlpha(source, 0, 0);
  SDL_SetAlpha(sub_image, 0, 0);
 
  SDL_Rect src_rect = {x, 0, frame_width, frame_height};
  SDL_Rect dst_rect = {0, 0, frame_width, frame_height };
  for(int i = 0; i < clip.frame_count; i++) {
    SDL_BlitSurface(source, &src_rect, sub_image, &dst_rect);
    src_rect.y += frame_height;
    clip.frames.push_back(getGLTexture(sub_image));
    SDL_FillRect(sub_image, &dst_rect, SDL_MapRGBA(sub_image->format, 0, 0, 0, 0));
  }
  SDL_FreeSurface(sub_image);
}

Animation::Animation(const char* fname) {
  std::fstream ifile(fname, std::fstream::in);
  char image_fname[80];
  ifile.getline(image_fname,80);

  if((ifile >> frame_width >> frame_height).bad()) {
    std::cerr << "Error loading " << fname << ": file terminated early." << std::endl;
  }
  
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

  if(clips.size() == 0) {
    std::cerr << "Error loading " << fname << ": empty clip information." << std::cerr;
  }
}

void AnimationTimer::advanceFrame() {
  current_frame++;
  if(current_frame >= animation->getFrameCount(current_clip)) {
    if(next_clip>=0) {
      current_frame = 0;
      current_clip = next_clip;
    }
    else {
      stop();
      current_frame--;
    }
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

void AnimationTimer::playClipOnce(int id, int next_id) {
  if(0 <= id && id < animation->getClipCount()) {
    next_clip = next_id;
    current_clip = id;
    timer = 0;
    current_frame = 0;
  }
}

void AnimationTimer::setClip(int id) {
  if(0 <= id && id < animation->getClipCount()) {
    next_clip = id;
    current_clip = id;
    timer = 0;
    current_frame = 0;
  }
}

void AnimationTimer::start() {
  timer = animation->getDuration(current_clip);
  status = PLAYING;
}

void AnimationTimer::stop() {
  status = STOPPED;
}

AnimationTimer::AnimationTimer() 
  : current_clip(0), next_clip(0), current_frame(0), timer(0), status(0), animation(NULL) {
}

AnimationTimer::AnimationTimer(Animation* animation)
  : current_clip(0), next_clip(0), current_frame(0), timer(0), status(0), animation(animation) {
}
