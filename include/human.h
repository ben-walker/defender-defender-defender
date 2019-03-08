#ifndef HUMAN_HERD_
#define HUMAN_HERD_

#include "point.h"
#include <stdbool.h>

enum MAX_HUMANS { MAX_HUMANS = 10 };
enum NAME_LEN { NAME_LEN = 25 };

typedef struct Human {
   Point head, torso, legs;
   char name[NAME_LEN];
   bool captive;
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

void applyHumanGravity();

void shootHuman(const int index);

int humanAtPoint(const float x, const float y, const float z);

Human *getHumans();

int currentHumans();

Human *findNearbyHuman(Point start, const float maxDist);

void markCaptive(const char *name);

#endif // HUMAN_HERD_
