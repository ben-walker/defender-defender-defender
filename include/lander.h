#ifndef LANDER_
#define LANDER_

#include "human.h"
#include "point.h"
#include <stdbool.h>

enum MAX_LANDERS { MAX_LANDERS = 10 };
enum LANDER_NAME_LEN { LANDER_NAME_LEN = 25 };
enum State {
   search,
   pursue,
   kidnap,
   reset,
   attack,
   bounce
};

typedef struct Lander {
   int id;
   Point center, targetPosition;
   Human *target;
   enum State state, prevState;
   float xVec, zVec;
   const char *name;
   bool super, dead;
   long lastAttack, lastRecal, bounceStart;
} Lander;

static const char *LANDER_NAMES[] = {
   "Ctrl",
   "Alt",
   "Del",
   "Tilde",
   "Esc",
   "Backspace",
   "Tab",
   "Fn",
   "Num Lock",
   "Pg Up"
};

void spawnLander();

void articulateLanders();

Lander *getLanders();

int currentLanders();

void shootLander(const int index);

int landerAtPoint(Point point);

#endif // LANDER_
