#ifndef LANDER_
#define LANDER_

#include "pointF.h"

enum MAX_LANDERS { MAX_LANDERS = 3 };
enum State {
   searching,
   pursuing,
   abducting
};

typedef struct Lander {
   enum State state;
   PointF center;
   float xVec, zVec;
} Lander;

void spawnLander();

void articulateLanders();

Lander *getLanders();

int currentLanders();

void shootLander(const int index);

int landerAtPoint(const float fx, const float fy, const float fz);

#endif // LANDER_
