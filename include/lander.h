#ifndef LANDER_
#define LANDER_

#include "human.h"
#include "point.h"
#include <stdbool.h>

enum MAX_LANDERS { MAX_LANDERS = 3 };
enum LANDER_NAME_LEN { LANDER_NAME_LEN = 25 };
enum State {
   search,
   pursue,
   kidnap,
   reset,
   attack
};

typedef struct Lander {
   Point center;
   Human *target;
   enum State state;
   float xVec, zVec;
   const char *name;
   bool super;
} Lander;

static const char *LANDER_NAMES[] = {
   "Zeke Teenweed",
   "Borth Sampson",
   "Jerry Potter"
};

void spawnLander();

void articulateLanders();

Lander *getLanders();

int currentLanders();

void shootLander(const int index);

int landerAtPoint(const float fx, const float fy, const float fz);

#endif // LANDER_
