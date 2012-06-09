cmake_minimum_required (VERSION 2.6)
project (pete-redux)

if(WIN32)
	link_directories(src)
	set (EXTRA_LIBS ${EXTRA_LIBS} BaseCode ClientCode SDL SDLmain
	SDL_image GLU32 OpenGL32 BulletDynamics BulletCollision LinearMath)
elseif(UNIX)
	include_directories("/usr/local/include/bullet" "/usr/include/freetype2")
	set (EXTRA_LIBS ${EXTRA_LIBS} BaseCode ClientCode SDL SDL_image GLU GL
	GLEW freetype BulletDynamics BulletCollision LinearMath)
endif()


set (pete-redux_VERSION_MAJOR 0)
set (pete-redux_VERSION_MINOR 2)

add_subdirectory(src)
