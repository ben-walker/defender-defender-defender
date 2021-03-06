#ifndef HUMAN_HERD_
#define HUMAN_HERD_

#include "point.h"
#include <stdbool.h>

enum MAX_HUMANS { MAX_HUMANS = 10 };
enum NAME_LEN { NAME_LEN = 25 };

typedef struct Human {
   Point head, torso, legs;
   const char *name;
   bool captive, dead;
   float fallStart;
} Human;

static const char *NAMES[] = {
   "Dan",
   "Steven",
   "Jack",
   "Bob",
   "Jim",
   "Rebecca",
   "Susan",
   "Morgan",
   "Emily",
   "Olivia"
};

void spawnHuman();

void spawnHumanCoordinates(const int x, const int y, const int z);

void applyHumanGravity();

void shootHuman(const int index);

int humanAtPoint(Point point);

void adjustHumanByVector(Human *human, Point vector);

Human *getHumans();

int currentHumans();

Human *findNearbyHuman(Point start, const float maxDist);

void markCaptive(const char *name);

void abductHuman(const char *name);

#endif // HUMAN_HERD_
