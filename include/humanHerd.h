#ifndef HUMAN_HERD_
#define HUMAN_HERD_

enum MAX_HUMANS { MAX_HUMANS = 10 };
enum NAME_LEN { NAME_LEN = 25 };

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

void spawnHuman(int x, int y, int z);

void applyHumanGravity();

void humanAtPoint(const float x, const float y, const float z);

#endif // HUMAN_HERD_
