#ifndef DAMAGETAKER_H_
#define DAMAGETAKER_H_

class DamageTaker {
 public:
  virtual void takeDamage(int damage, int type)=0;
};

#endif
