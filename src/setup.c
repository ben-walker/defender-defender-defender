#include "setup.h"
#include "graphics.h"
#include "humanHerd.h"

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
extern void setViewPosition(float, float, float);
extern void setViewOrientation(float, float, float);

static const int Y_ORI = 135;

void initializeWorld() {
   for (int i = 0; i < WORLDX; i++)
      for (int j = 0; j < WORLDY; j++)
         for (int k = 0; k < WORLDZ; k++)
            world[i][j][k] = 0;
}

void setStartPosition() {
   setViewPosition(WORLDX - 1, -WORLDY / 2, WORLDZ - 1);
   setViewOrientation(0, Y_ORI, 0);
}

void spawnSomeFolks() {
   spawnHuman(10, 47, 10);
   spawnHuman(20, 47, 20);
   spawnHuman(90, 47, 10);
   spawnHuman(75, 47, 75);
   spawnHuman(30, 47, 20);
}
