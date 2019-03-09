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
   spawnHumanCoordinates(48, 0, 59);
   spawnHumanCoordinates(52, 0, 27);
   spawnHumanCoordinates(14, 0, 20);
   spawnHumanCoordinates(18, 0, 61);
   spawnHumanCoordinates(32, 0, 30);
   spawnHumanCoordinates(18, 0, 28);
   spawnHumanCoordinates(25, 0, 65);
   spawnHumanCoordinates(65, 0, 56);
   spawnHumanCoordinates(46, 0, 80);
   spawnHumanCoordinates(32, 0, 51);

   for (int i = 0; i < MAX_LANDERS; i += 1)
      spawnLander();
}

void setup() {
   srand(time(NULL));
   initializeWorld();
   setStartPosition();
   buildHeightmapFrom(GROUND_FILE);
   spawnHumansAndAliens();
}
