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

float endPoint(const float pointStart, const float pointChange, const float dist) {
   return pointStart - pointChange * dist;
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

void populateCurrentRayCoordinates(const float bx, const float by, const float bz,
   const float ex, const float ey, const float ez) {
   rays[rayIndex].start.x = bx;
   rays[rayIndex].start.y = by;
   rays[rayIndex].start.z = bz;
   rays[rayIndex].end.x = ex;
   rays[rayIndex].end.y = ey;
   rays[rayIndex].end.z = ez;
}

void buildRayUnits(Point start, Point end) {
   float ex, ey, ez;
   int humanIndex, landerIndex;

   for (float i = 0.5; i < RAY_DIST; i += 0.5) {
      ex = endPoint(start.x, end.x, i);
      ey = endPoint(start.y, end.y, i);
      ez = endPoint(start.z, end.z, i);
      createTube(rayIndex, start.x, start.y, start.z, ex, ey, ez, RAY_COLOR);
      if ((humanIndex = humanAtPoint(ex, ey, ez)) != -1)
         shootHuman(humanIndex);
      if ((landerIndex = landerAtPoint(ex, ey, ez)) != -1)
         shootLander(landerIndex);
   }
   populateCurrentRayCoordinates(start.x, start.y, start.z, ex, ey, ez);
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
