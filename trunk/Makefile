DIRFLAGS = -L$(HOME)/lib -I$(HOME)/include -I/usr/local/include/bullet
CPPFLAGS=-g $(DIRFLAGS)
CFLAGS=-g -std=c99
TARGET=pete
OBJS=petemain.o level.o tileset.o texture.o player.o animation.o ctvec3.o gamestate.o zombie.o gameobject.o platformai.o bulletwrap.o
LINK=g++ $(CPPFLAGS)
#LINK=gcc $(CFLAGS)
LFLAGS=-lSDL -lSDL_image -lGLU -lGL -lBulletDynamics -lBulletCollision -lLinearMath

.SUFFIXES:
.SUFFIXES: .d .o .h .c .cc .C .cpp
.c.o: ; $(CC) $(CFLAGS) -MMD -c $*.c
.cc.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.cc 
.C.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.C
.cpp.o: ; $(CPP) $(CPPFLAGS) -MMD -c $*.cpp

CC=gcc
CPP=g++

%.d: %.c
	touch $@
%.d: %.cc
	touch $@
%.d: %.C
	touch $@
%.d: %.cpp
	touch $@

DEPENDENCIES = $(OBJS:.o=.d)

# 
# Targets:
# 
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LINK) $(FLAGS) -o $(TARGET) $(OBJS) $(DIRFLAGS) $(LFLAGS)

clean:
	-rm -f $(TARGET) $(OBJS) $(DEPENDENCIES) make.dep


make.dep: $(DEPENDENCIES)
	-cat $(DEPENDENCIES) > make.dep

run: all
	./$(TARGET)
include make.dep


