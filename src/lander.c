#include "lander.h"
#include "graphics.h"
#include "vpOps.h"
#include "raygun.h"
#include "timeAssistant.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
extern void getViewPosition(float *, float *, float *);

static const int SEARCH_HEIGHT = 35,
   BODY_COLOR = 6,
   SU_BODY_COLOR = 1,
   ATTACK_RANGE = 40,
   ATTACK_FREQ = 1500, // ms
   RECAL_FREQ = 400; // ms
static const float RANGE = 10.0,
   PURSUIT_MOD = 30.0,
   PL_PURSUIT_MOD = 200.0;

static Lander landers[MAX_LANDERS];
static int numLanders = 0;

float randF() {
   return (float) rand() / (float) RAND_MAX;
}

bool actionReady(const long ts, const long freq) {
   return getMsTimestamp() - ts > freq;
}

bool topOfWorld(Lander lander) {
   return lander.center.y + 2 > WORLDY;
}

Lander getNewLander() {
   Point center = { rand() % WORLDX, SEARCH_HEIGHT, rand() % WORLDZ };
   Lander newLander = {
      .state = search,
      .center = center,
      .xVec = randF(),
      .zVec = randF(),
      .name = LANDER_NAMES[numLanders]
   };
   return newLander;
}

bool pointInsideLander(Lander lander, Point point) {
   for (int y = lander.center.y; y < lander.center.y + 3; y += 1)
      for (int x = lander.center.x - 1; x < lander.center.x + 2; x += 1)
         for (int z = lander.center.z - 1; z < lander.center.z + 2; z += 1)
            if (pointsClose((Point) { x, y, z }, point)) return true;
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

int landerAtPoint(Point point) {
   for (int i = 0; i < numLanders; i += 1)
      if (pointInsideLander(landers[i], point)) return i;
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

void moveLander(Lander *lander, Point movementVector) {
   eraseLander(*lander);
   lander->center = addPoints(lander->center, movementVector);
   corralLander(lander);
   drawLander(*lander);
}

void ambientMovement(Lander *lander) {
   moveLander(lander, (Point) { lander->xVec, 0, lander->zVec });
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
   moveLander(lander, pointDivision(vector, PURSUIT_MOD));
}

void pursuePlayer(Lander *lander) {
   Point lerpedCenter = { (int) lander->center.x, (int) lander->center.y, (int) lander->center.z };
   Point vector = vectorBetween(lerpedCenter, absViewPos());
   moveLander(lander, pointDivision(vector, PL_PURSUIT_MOD));
}

void performAbduction(Lander *lander) {
   printf("%s abducted %s!\n", lander->name, lander->target->name);
   abductHuman(lander->target->name);
   lander->super = true;
   lander->state = attack;
}

void abduct(Lander *lander) {
   if (lander->target->dead) {
      lander->target = NULL;
      lander->state = reset;
      return;
   }
   moveLander(lander, (Point) { 0, 0.15, 0 });
   topOfWorld(*lander)
      ? performAbduction(lander)
      : adjustHumanByVector(lander->target, (Point) { 0, 0.15, 0 });
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

void shootAtPlayer(Lander *lander) {
   if (!actionReady(lander->lastAttack, ATTACK_FREQ))
      return;
   Point vector = vectorBetween(lander->targetPosition, lander->center);
   float magnitude = vectorMagnitude(vector);
   int distance = magnitude >= ATTACK_RANGE ? ATTACK_RANGE : magnitude;
   fireRayFromPoint(lander->center, unitVector(vector), distance);
   lander->lastAttack = getMsTimestamp();
}

void recalibrateRay(Lander *lander) {
   if (!actionReady(lander->lastRecal, RECAL_FREQ))
      return;
   lander->targetPosition = absViewPos();
   lander->lastRecal = getMsTimestamp();
}

void attackPlayer(Lander *lander) {
   pursuePlayer(lander);
   recalibrateRay(lander);
   shootAtPlayer(lander);
}

void articulateLanders() {
   for (int i = 0; i < numLanders; i += 1) {
      Lander *lander = &landers[i];
      switch (lander->state) {
         case search:
            ambientMovement(lander);
            scanHorizon(lander);
            break;
         
         case pursue:
            pursueTarget(lander);
            break;

         case kidnap:
            abduct(lander);
            break;

         case reset:
            resetToSearchState(lander);
            break;

         case attack:
            attackPlayer(lander);
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
