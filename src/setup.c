#include "setup.h"
#include "graphics.h"
#include "humanHerd.h"
#include <time.h>
#include <stdlib.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
extern void setViewPosition(float, float, float);
extern void setViewOrientation(float, float, float);

static const int Y_ORI = 135;
static const int MAX_HUMAN_Y = 47;
static const int MIN_HUMAN_Y = 20;

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
   int ymod = (MAX_HUMAN_Y - MIN_HUMAN_Y + 1) + MIN_HUMAN_Y;
   srand(time(NULL));
   for (int i = 0; i < MAX_HUMANS; i++)
      spawnHuman(rand() % WORLDX, rand() % ymod, rand() % WORLDZ);
}
