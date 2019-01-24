#include "humanHerd.h"
#include "graphics.h"
#include <stdbool.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
static const int HEAD_COLOR = 1;
static const int TORSO_COLOR = 3;
static const int LEGS_COLOR = 7;

enum MAX_HUMANS { MAX_HUMANS = 10 };
static Human humans[MAX_HUMANS];
static int numHumans = 0;

bool occupied(int x, int y, int z) {
   return world[x][y][z] != 0;
}

int findAvailableY(const int x, int y, const int z) {
   while (world[x][y][z] != 0)
      y += 1;
   return y;
}

Human getNewHuman(const int x, const int botY, const int z) {
   Point legs = { x, botY, z, LEGS_COLOR };
   Point torso = { x, botY + 1, z, TORSO_COLOR };
   Point head = { x, botY + 2, z, HEAD_COLOR };
   return (Human) { head, torso, legs };
}

void addToWorld(Human newHuman) {
   int x = newHuman.legs.x, z = newHuman.legs.z;
   world[x][newHuman.legs.y][z] = newHuman.legs.color;
   world[x][newHuman.torso.y][z] = newHuman.torso.color;
   world[x][newHuman.head.y][z] = newHuman.head.color;
   humans[numHumans] = newHuman;
   numHumans++;
}

void spawnHuman(int x, int y, int z) {
   if (numHumans == MAX_HUMANS)
      return;
   if (occupied(x, y, z))
      y = findAvailableY(x, y, z);
   addToWorld(getNewHuman(x, y, z));
}
