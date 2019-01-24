#ifndef HUMAN_HERD_
#define HUMAN_HERD_

enum MAX_HUMANS { MAX_HUMANS = 10 };

typedef struct Point {
   int x;
   int y;
   int z;
   int color;
} Point;

typedef struct Human {
   Point head;
   Point torso;
   Point legs;
} Human;

void spawnHuman(int x, int y, int z);

void applyHumanGravity();

#endif // HUMAN_HERD_
