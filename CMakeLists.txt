cmake_minimum_required (VERSION 2.6)
project (pete-redux)

include_directories("/usr/local/include/bullet")

set (EXTRA_LIBS ${EXTRA_LIBS} BaseCode ClientCode SDL SDL_image GLU GL BulletDynamics BulletCollision LinearMath)
set (pete-redux_VERSION_MAJOR 0)
set (pete-redux_VERSION_MINOR 1)

add_subdirectory(src)

add_executable(pete src/petemain.cpp)
target_link_libraries(pete ${EXTRA_LIBS})
