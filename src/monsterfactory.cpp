#include "objecttypes.h"
#include "bullet.h"

namespace MonsterFactory {
  GameObject* createMonster(int id, const btVector3& position, GameState* gamestate) {
    switch(id) {
    case 0:
      DEBUG_OUT("New Zombie");
      return new Zombie(gamestate, position);
    case 1:
      DEBUG_OUT("New Bat");
      return new Bat(gamestate, position);
    case 2:
      DEBUG_OUT("New Soul");
      return new Soul(gamestate, position);
    case 3:
      DEBUG_OUT("New DarkChampion");
      return new DarkChampion(gamestate, position);
      break;
    case 4:
      DEBUG_OUT("New Soldier");
      return (new Soldier(gamestate, position));
    case 21:
      DEBUG_OUT("New Exit");
      return (new Exit(gamestate, position));
    case 98:
      DEBUG_OUT("New Medpack");
      return (new Medpack(gamestate, position));
    case 99:
      DEBUG_OUT("New Ammo");
      return (new Ammo(gamestate, position));
    }
    return NULL;
  }
};
