#include "lander.h"
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

static Lander landers[MAX_LANDERS];
static int numLanders = 0;
static const int SEARCH_HEIGHT = 40;
static const float RANGE = 10.0;
static const float PURSUIT_SPEED = 30.0;

float randFl() {
   return (float) rand() / (float) RAND_MAX;
}

Lander getNewLander() {
   Point center = { rand() % WORLDX, SEARCH_HEIGHT, rand() % WORLDZ };
   Lander newLander = {
      .state = searching,
      .center = center,
      .xVec = randFl(),
      .zVec = randFl(),
      .name = LANDER_NAMES[numLanders],
      .target = NULL
   };
   return newLander;
}

bool pointInsideLander(Lander lander, Point point) {
   int x = lander.center.x, y = lander.center.y, z = lander.center.z;
   for (int w = y; w < y + 3; w += 1) {
      for (int k = x - 1; k < x + 2; k+= 1) {
         for (int j = z - 1; j < z + 2; j += 1) {
            Point testPoint = { k, w, j };
            if (pointsClose(testPoint, point))
               return true;
         }
      }
   }
   return false;
}

void drawTopOfLander(Point center) {
   int x = center.x, y = center.y, z = center.z;
   for (int w = y + 1; w < y + 3; w += 1)
      for (int i = x - 1; i < x + 2; i+= 1)
         for (int j = z - 1; j < z + 2; j += 1)
            world[i][w][j] = 6;
}

void addEyes(Point center) {
   int y = center.y + 1, x = center.x, z = center.z;
   world[x + 1][y][z + 1] = 4;
   world[x + 1][y][z - 1] = 4;
   world[x - 1][y][z + 1] = 4;
   world[x - 1][y][z - 1] = 4;
}

void addLegs(Point center) {
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
   Point center = lander.center;
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

void deleteLanderAt(int index) {
   for (; index < numLanders - 1; index += 1)
      landers[index] = landers[index + 1];
   numLanders -= 1;
}

void spawnLander() {
   if (numLanders == MAX_LANDERS)
      return;
   Lander lander = getNewLander();
   drawLander(lander);
   trackLanders(lander);
}

void shootLander(const int index) {
   printf("You killed %s!\n", landers[index].name);
   eraseLander(landers[index]);
   if (landers[index].target != NULL)
      landers[index].target->captive = false;
   deleteLanderAt(index);
}

int landerAtPoint(const float fx, const float fy, const float fz) {
   Point point = { fx, fy, fz };

   for (int i = 0; i < numLanders; i += 1) {
      if (pointInsideLander(landers[i], point))
         return i;
   }
   return -1;
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

void scanHorizon(Lander *lander) {
   Human *victim = findNearbyHuman(lander->center, RANGE);
   if (victim == NULL)
      return;
   lander->state = pursuing;
   lander->target = victim;
}

Point getVectorBetween(Point start, Point end) {
   return (Point) { end.x - start.x, end.y - start.y, end.z - start.z };
}

void pursueTarget(Lander *lander) {
   Point vector = getVectorBetween(
      (Point) { lander->center.x, lander->center.y - 1, lander->center.z },
      (Point) { (float) lander->target->head.x, (float) lander->target->head.y, (float) lander->target->head.z }
   );
   if (fabs(vector.x) < 0.1 && fabs(vector.y) < 0.1 && fabs(vector.z) < 0.1) {
      lander->state = abducting;
      markCaptive(lander->target->name);
      return;
   }
   eraseLander(*lander);
   lander->center.x += vector.x / PURSUIT_SPEED;
   lander->center.y += vector.y / PURSUIT_SPEED;
   lander->center.z += vector.z / PURSUIT_SPEED;
   corralLander(lander);
   drawLander(*lander);
}

void abduct(Lander *lander) {
   eraseLander(*lander);
   lander->center.y += 0.15;
   corralLander(lander);
   drawLander(*lander);
   adjustHumanByVector(lander->target, (Point) { 0, 0.15, 0 });
}

void articulateLanders() {
   for (int i = 0; i < numLanders; i += 1) {
      switch (landers[i].state) {
         case searching:
            ambientMovement(&landers[i]);
            scanHorizon(&landers[i]);
            break;
         
         case pursuing:
            pursueTarget(&landers[i]);
            break;

         case abducting:
            abduct(&landers[i]);
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
