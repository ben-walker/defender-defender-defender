#ifndef HUMAN_HERD_
#define HUMAN_HERD_

#include "point.h"

enum MAX_HUMANS { MAX_HUMANS = 10 };
enum NAME_LEN { NAME_LEN = 25 };

typedef struct Human {
   Point head;
   Point torso;
   Point legs;
   char name[NAME_LEN];
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

void humanAtPoint(const float x, const float y, const float z);

Human *getHumans();

#endif // HUMAN_HERD_
