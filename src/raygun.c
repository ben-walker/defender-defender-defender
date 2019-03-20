#include "raygun.h"
#include "timeAssistant.h"
#include "human.h"
#include "lander.h"
#include "vpOps.h"
#include <stdio.h>
#include <math.h>

static const int RAY_COLOR = 6; // pink
static const int RAY_DIST = 150;
static const int RAY_LIFESPAN = 350;
static Ray rays[RAY_COUNT];
static int rayIndex = -1;

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);

bool rayInvalid(Ray ray) {
   return (ray.spawnTime + RAY_LIFESPAN) < getMsTimestamp() && ray.active;
}

int nextRay() {
   return (rayIndex == RAY_COUNT - 1) ? 0 : rayIndex + 1;
}

Ray getNewRay() {
   Ray newRay = {
      .id = rayIndex,
      .spawnTime = getMsTimestamp(),
      .active = true
   };
   return newRay;
}

void populateCurrentRayCoordinates(Point start, Point end) {
   rays[rayIndex].start = start;
   rays[rayIndex].end = end;
}

void buildRayUnits(Point start, Point change) {
   Point end;
   int humanIndex, landerIndex;

   for (float i = 0.5; i < RAY_DIST; i += 0.5) {
      end = getEndPoint(start, change, i);
      createTube(rayIndex, start.x, start.y, start.z, end.x, end.y, end.z, RAY_COLOR);
      if ((humanIndex = humanAtPoint(end.x, end.y, end.z)) != -1)
         shootHuman(humanIndex);
      if ((landerIndex = landerAtPoint(end.x, end.y, end.z)) != -1)
         shootLander(landerIndex);
   }
   populateCurrentRayCoordinates(start, end);
}

void fireRayFromVP() {
   rayIndex = nextRay();
   rays[rayIndex] = getNewRay();
   buildRayUnits(absViewPos(), nextPos());
}

void fireRayFromPoint(Point start, Point end) {
}

void fizzleRays() {
   for (int i = 0; i < RAY_COUNT; i += 1)
      if (rayInvalid(rays[i])) {
         rays[i].active = false;
         hideTube(rays[i].id);
      }
}

Ray *getRays() {
   return rays;
}
