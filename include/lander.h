#ifndef LANDER_
#define LANDER_

#include "point.h"

enum MAX_LANDERS { MAX_LANDERS = 5 };
enum State {
   searching,
   pursuing,
   abducting
};

typedef struct Lander {
   enum State state;
   Point center;
   float xVec, zVec;
} Lander;

void spawnLander();

void articulateLanders();

Lander *getLanders();

int currentLanders();

#endif // LANDER_
