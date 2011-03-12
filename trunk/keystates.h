#ifndef KEYSTATES_H_
#define KEYSTATES_H_
struct KeyStates {
  bool right_held;
  bool left_held;
  bool up_held;
  bool down_held;
  bool space_held;
  KeyStates();
};

#endif
