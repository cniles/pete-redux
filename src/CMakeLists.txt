#configure_file (
#	       "${PROJECT_SOURCE_DIR}/src/peteconfig.h.in"
#	       "${PROJECT_BINARY_DIR}/src/peteconfig.h"
#)

add_library(BaseCode animation.cpp gameobject.cpp physicsobject.cpp
		       statemachineobject.cpp bullet.cpp gamestate.cpp 
		       level.cpp texture.cpp tileset.cpp)

add_library(ClientCode player.cpp zombie.cpp bat.cpp soul.cpp darkchampion.cpp)
target_link_libraries(ClientCode BaseCode)

add_executable(../pete petemain.cpp)
target_link_libraries(../pete ${EXTRA_LIBS})
