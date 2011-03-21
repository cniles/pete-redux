#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>

//#define DEBUG
//#define AI_DEBUG

#ifdef DEBUG
#define DEBUG_OUT(MESSAGE) std::cerr << MESSAGE << std::endl;
#else
#define DEBUG_OUT(MESSAGE)
#endif 

#ifdef AI_DEBUG
#define AI_DEBUG_OUT(MESSAGE) std::cerr << MESSAGE << std::endl;
#else
#define AI_DEBUG_OUT(MESSAGE) 
#endif


#endif
