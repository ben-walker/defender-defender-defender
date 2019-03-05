#include "lander.h"
#include "graphics.h"
#include "human.h"
#include <stdlib.h>
#include <stdio.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

static Lander landers[MAX_LANDERS];
static int numLanders = 0;
static const int SEARCH_HEIGHT = 40;
static const float RANGE = 10.0;

float randFl() {
   return (float) rand() / (float) RAND_MAX;
}

Lander getNewLander() {
   PointF center = { rand() % WORLDX, SEARCH_HEIGHT, rand() % WORLDZ };
   Lander newLander = {
      .state = searching,
      center,
      .xVec = randFl(),
      .zVec = randFl()
   };
   return newLander;
}

void drawTopOfLander(PointF center) {
   int x = center.x, y = center.y, z = center.z;
   for (int w = y + 1; w < y + 3; w += 1)
      for (int i = x - 1; i < x + 2; i+= 1)
         for (int j = z - 1; j < z + 2; j += 1)
            world[i][w][j] = 6;
}

void addEyes(PointF center) {
   int y = center.y + 1, x = center.x, z = center.z;
   world[x + 1][y][z + 1] = 4;
   world[x + 1][y][z - 1] = 4;
   world[x - 1][y][z + 1] = 4;
   world[x - 1][y][z - 1] = 4;
}

void addLegs(PointF center) {
   int x = center.x, y = center.y, z = center.z;
   world[x + 1][y][z + 1] = 8;
   world[x + 1][y][z - 1] = 8;
   world[x - 1][y][z + 1] = 8;
   world[x - 1][y][z - 1] = 8;
}

void drawLander(Lander lander) {
   drawTopOfLander(lander.center);
   addEyes(lander.center);
   addLegs(lander.center);
}

void eraseLander(Lander lander) {
   PointF center = lander.center;
   int x = center.x, y = center.y, z = center.z;
   for (int w = y; w < y + 3; w += 1)
      for (int i = x - 1; i < x + 2; i+= 1)
         for (int j = z - 1; j < z + 2; j += 1)
            world[i][w][j] = 0;
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

void corralLander(Lander *lander) {
   if (lander->center.x + 1 > WORLDX) {
      lander->center.x = WORLDX - 2;
      lander->xVec = -lander->xVec;
   } else if (lander->center.x < 1) {
      lander->center.x = 1;
      lander->xVec = -lander->xVec;
   }
   if (lander->center.z + 1 > WORLDZ) {
      lander->center.z = WORLDZ - 2;
      lander->zVec = -lander->zVec;
   } else if (lander->center.z < 1) {
      lander->center.z = 1;
      lander->zVec = -lander->zVec;
   }
}

void ambientMovement(Lander *lander) {
   eraseLander(*lander);
   lander->center.x += lander->xVec;
   lander->center.z += lander->zVec;
   corralLander(lander);
   drawLander(*lander);
}

void scanHorizon(Lander lander) {
   Human *victim = findNearbyHuman(lander.center, RANGE);
}

void articulateLanders() {
   for (int i = 0; i < numLanders; i += 1) {
      switch (landers[i].state) {
         case searching:
            ambientMovement(&landers[i]);
            scanHorizon(landers[i]);
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
