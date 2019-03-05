#include "lander.h"
#include "graphics.h"
#include <stdlib.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

static Lander landers[MAX_LANDERS];
static int numLanders = 0;
static const int SEARCH_HEIGHT = 40;

Lander getNewLander() {
   Point center = { rand() % WORLDX, SEARCH_HEIGHT, rand() % WORLDZ, 0 };
   Lander newLander = { searching, center };
   return newLander;
}

void drawTopOfLander(Point center) {
   for (int w = center.y + 1; w < center.y + 3; w += 1)
      for (int i = center.x - 1; i < center.x + 2; i+= 1)
         for (int j = center.z - 1; j < center.z + 2; j += 1)
            world[i][w][j] = 1;
}

void addEyes(Point center) {
   int y = center.y + 1;
   world[center.x + 1][y][center.z + 1] = 4;
   world[center.x + 1][y][center.z - 1] = 4;
   world[center.x - 1][y][center.z + 1] = 4;
   world[center.x - 1][y][center.z - 1] = 4;
}

void addLegs(Point center) {
   int y = center.y;
   world[center.x + 1][y][center.z + 1] = 8;
   world[center.x + 1][y][center.z - 1] = 8;
   world[center.x - 1][y][center.z + 1] = 8;
   world[center.x - 1][y][center.z - 1] = 8;
}

void drawLander(Lander lander) {
   drawTopOfLander(lander.center);
   addEyes(lander.center);
   addLegs(lander.center);
}

void trackLanders(Lander lander) {
   landers[numLanders] = lander;
   numLanders += 1;
}

void spawnLander() {
   if (numLanders == MAX_LANDERS)
      return;
   Lander lander = getNewLander();
   drawLander(lander);
   trackLanders(lander);
}

void articulateLanders() {
   for (int i = 0; i < numLanders; i += 1) {
      switch (landers[i].state) {
         case searching:
            break;
         
         case pursuing:
            break;

         case abducting:
            break;
      
         default:
            break;
      }
   }
}

Lander *getLanders() {
   return landers;
}

int currentLanders() {
   return numLanders;
}
