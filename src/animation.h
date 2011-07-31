#ifndef ANIMATION_H
#define ANIMATION_H
#include <SDL/SDL.h>
#include <vector>

class Animation;

class Clip {
  std::vector<unsigned int> frames;
  int frame_duration;
  int frame_count;
 public:
  Clip(int, int);
  friend class Animation;
};

class Animation {
  std::vector<Clip> clips;
  int frame_width;
  int frame_height;
  void parseClip(int index, Clip& clip, SDL_Surface* source);
public:
  Animation();
  Animation(const char* fname);
  inline int getWidth() const { return frame_width; }
  inline int getHeight() const { return frame_height; }
  inline int getFrame(int clip, int frame) const { return clips[clip].frames[frame]; }
  inline int getDuration(int clip) { return clips[clip].frame_duration; }
  inline int getFrameCount(int clip) { return clips[clip].frame_count; }
  inline int getClipCount() { return clips.size(); }
};

class AnimationTimer {
  Animation* animation;
  int next_clip;
  int current_clip;
  int current_frame;
  int timer;
  int status;  
  void advanceFrame();
 public:
  AnimationTimer();
  AnimationTimer(Animation*);
  static const int STOPPED = 0;
  static const int PLAYING = 1;
  static const int PAUSED = 2;
  inline int getFrame() const { return animation->getFrame(current_clip, current_frame); }
  inline int getState() { return status; }

  void start();
  void stop();
  void playClipOnce(int id, int next_id);
  void setClip(int id);
  void tick(Uint32 dt);
};

#endif
