#ifndef MONSTER_FACTORY_H_ 
#define MONSTER_FACTORY_H_

class GameObject;
class GameState;
namespace MonsterFactory {
  GameObject* createMonster(int, const btVector3&, GameState*);
}

#endif
