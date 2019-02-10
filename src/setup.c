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
static const int MIN_HUMAN_Y = 35;

void initializeWorld() {
   for (int i = 0; i < WORLDX; i++)
      for (int j = 0; j < WORLDY; j++)
         for (int k = 0; k < WORLDZ; k++)
            world[i][j][k] = 0;
}

void setStartPosition() {
   setViewPosition(-WORLDX + 95, -WORLDY / 2, -WORLDZ + 90);
   setViewOrientation(0, Y_ORI, 0);
}

void spawnSomeFolks() {
   int yrange = (MAX_HUMAN_Y - MIN_HUMAN_Y + 1);
   srand(time(NULL));
   for (int i = 0; i < MAX_HUMANS; i++)
      spawnHuman(rand() % WORLDX, rand() % yrange + MIN_HUMAN_Y, rand() % WORLDZ);
}
