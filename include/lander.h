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
   int id;
   Point center, targetPosition;
   Human *target;
   enum State state;
   float xVec, zVec;
   const char *name;
   bool super;
   long lastAttack, lastRecal;
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

int landerAtPoint(Point point);

#endif // LANDER_
