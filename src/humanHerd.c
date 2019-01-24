#include "humanHerd.h"
#include "graphics.h"
#include <stdbool.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
static const int HEAD = 1;
static const int TORSO = 3;
static const int LEGS = 7;
static const int BODY[] = {LEGS, TORSO, HEAD};

bool occupied(int x, int y, int z) {
   return world[x][y][z] != 0;
}

int findAvailableY(const int x, int y, const int z) {
   while (world[x][y][z] != 0)
      y += 1;
   return y;
}

int height() {
   return sizeof(BODY) / sizeof(BODY[0]);
}

void spawnHuman(int x, int y, int z) {
   if (occupied(x, y, z))
      y = findAvailableY(x, y, z);
   for (int i = height() - 1; i >= 0; i--)
      world[x][y + i][z] = BODY[i];
}