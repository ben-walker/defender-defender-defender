#include "setup.h"
#include "graphics.h"
#include "human.h"
#include "lander.h"
#include "pgmTranslate.h"
#include <time.h>
#include <stdlib.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
extern void setViewPosition(float, float, float);
extern void setViewOrientation(float, float, float);

static const int Y_ORI = 135;
static const char *GROUND_FILE = "ground.pgm";

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
   for (int i = 0; i < MAX_HUMANS; i++)
      spawnHuman();
}

void spawnHumansAndAliens() {
   for (int i = 0; i < MAX_HUMANS; i += 1)
      spawnHuman();
   for (int i = 0; i < 1; i += 1)
      spawnLander();
}

void setup() {
   srand(time(NULL));
   initializeWorld();
   setStartPosition();
   buildHeightmapFrom(GROUND_FILE);
   spawnHumansAndAliens();
}
