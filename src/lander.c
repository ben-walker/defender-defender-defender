#include "lander.h"
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
extern void getViewPosition(float *, float *, float *);

static Lander landers[MAX_LANDERS];
static int numLanders = 0;
static const int SEARCH_HEIGHT = 35;
static const int BODY_COLOR = 6;
static const int SU_BODY_COLOR = 1;
static const float RANGE = 10.0;
static const float PURSUIT_MOD = 30.0;
static const float PL_PURSUIT_MOD = 200.0;

Point viewPosAsPoint() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return (Point) { -x, -y, -z };
}

float randFl() {
   return (float) rand() / (float) RAND_MAX;
}

Lander getNewLander() {
   Point center = { rand() % WORLDX, SEARCH_HEIGHT, rand() % WORLDZ };
   Lander newLander = {
      .state = search,
      .center = center,
      .xVec = randFl(),
      .zVec = randFl(),
      .name = LANDER_NAMES[numLanders],
      .target = NULL,
      .super = false
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

void drawTopOfLander(Point center, bool super) {
   int x = center.x, y = center.y, z = center.z;
   int color = super ? SU_BODY_COLOR : BODY_COLOR;

   for (int w = y + 1; w < y + 3; w += 1)
      for (int i = x - 1; i < x + 2; i+= 1)
         for (int j = z - 1; j < z + 2; j += 1)
            world[i][w][j] = color;
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
   drawTopOfLander(lander.center, lander.super);
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

void deleteLanderByName(const char *name) {
   int i;
   for (i = 0; i < numLanders; i += 1) {
      if (strcmp(name, landers[i].name) == 0)
         break;
   }
   if (i == numLanders)
      return;
   deleteLanderAt(i);
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
   lander->state = pursue;
   lander->target = victim;
}

void pursueTarget(Lander *lander) {
   if (lander->target->dead || lander->target->captive) {
      lander->target = NULL;
      lander->state = reset;
      return;
   }
   Point lerpedCenter = { (int) lander->center.x, (int) lander->center.y, (int) lander->center.z };
   Point vector = vectorBetween(lerpedCenter, lander->target->head);
   if (fabs(vector.y) < 1.0) {
      lander->state = kidnap;
      markCaptive(lander->target->name);
      return;
   }
   eraseLander(*lander);
   lander->center.x += vector.x / PURSUIT_MOD;
   lander->center.y += vector.y / PURSUIT_MOD;
   lander->center.z += vector.z / PURSUIT_MOD;
   corralLander(lander);
   drawLander(*lander);
}

void pursuePlayer(Lander *lander) {
   Point lerpedCenter = { (int) lander->center.x, (int) lander->center.y, (int) lander->center.z };
   Point vector = vectorBetween(lerpedCenter, viewPosAsPoint());
   eraseLander(*lander);
   lander->center.x += vector.x / PL_PURSUIT_MOD;
   lander->center.y += vector.y / PL_PURSUIT_MOD;
   lander->center.z += vector.z / PL_PURSUIT_MOD;
   corralLander(lander);
   drawLander(*lander);
}

void abduct(Lander *lander) {
   if (lander->target->dead) {
      lander->target = NULL;
      lander->state = reset;
      return;
   }
   eraseLander(*lander);
   lander->center.y += 0.15;
   if (lander->center.y + 2 > WORLDY) {
      printf("%s abducted %s!\n", lander->name, lander->target->name);
      abductHuman(lander->target->name);
      lander->super = true;
      lander->state = attack;
      return;
   }
   corralLander(lander);
   drawLander(*lander);
   adjustHumanByVector(lander->target, (Point) { 0, 0.15, 0 });
}

void resetToSearchState(Lander *lander) {
   eraseLander(*lander);
   if ((int) lander->center.y > SEARCH_HEIGHT)
      lander->center.y -= 0.15;
   else if ((int) lander->center.y < SEARCH_HEIGHT)
      lander->center.y += 0.15;
   else
      lander->state = search;
   corralLander(lander);
   drawLander(*lander);
}

void attackPlayer(Lander *lander) {
   pursuePlayer(lander);
}

void articulateLanders() {
   for (int i = 0; i < numLanders; i += 1) {
      switch (landers[i].state) {
         case search:
            ambientMovement(&landers[i]);
            scanHorizon(&landers[i]);
            break;
         
         case pursue:
            pursueTarget(&landers[i]);
            break;

         case kidnap:
            abduct(&landers[i]);
            break;

         case reset:
            resetToSearchState(&landers[i]);
            break;

         case attack:
            attackPlayer(&landers[i]);
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
