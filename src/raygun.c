#include "raygun.h"
#include "timeAssistant.h"
#include "human.h"
#include "lander.h"
#include <stdio.h>
#include <math.h>

static const float RAD_CONV = M_PI / 180.0;
static const int RAY_COLOR = 6; // pink
static const int RAY_DIST = 150;
static const int RAY_TIMER = 350;
static Ray rays[RAY_COUNT];
static int rayIndex = -1;

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);

float rads(const float deg) {
   return deg * RAD_CONV;
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

void correctedViewPos(float *x, float *y, float *z) {
   getViewPosition(x, y, z);
   *x = -*x; *y = -*y; *z = -*z;
}

void differentials(float *x, float *y, float *z) {
   float xrot, yrot, zrot;
   getViewOrientation(&xrot, &yrot, &zrot);
   *x = sinf(rads(-yrot));
   *y = sinf(rads(xrot));
   *z = cosf(rads(-yrot));
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

void buildRayUnits(const float bx, const float by, const float bz,
   const float xchange, const float ychange, const float zchange) {
   float ex, ey, ez;
   int humanIndex, landerIndex;

   for (float i = 0.5; i < RAY_DIST; i += 0.5) {
      ex = endPoint(bx, xchange, i);
      ey = endPoint(by, ychange, i);
      ez = endPoint(bz, zchange, i);
      createTube(rayIndex, bx, by, bz, ex, ey, ez, RAY_COLOR);
      if ((humanIndex = humanAtPoint(ex, ey, ez)) != -1)
         shootHuman(humanIndex);
      if ((landerIndex = landerAtPoint(ex, ey, ez)) != -1)
         shootLander(landerIndex);
   }
   populateCurrentRayCoordinates(bx, by, bz, ex, ey, ez);
}

void spawnRay() {
   float xchange, ychange, zchange, bx, by, bz;
   differentials(&xchange, &ychange, &zchange);
   correctedViewPos(&bx, &by, &bz);
   buildRayUnits(bx, by, bz, xchange, ychange, zchange);
}

void fireRayFromVP() {
   rayIndex = nextRay();
   rays[rayIndex] = getNewRay();
   spawnRay();
}

void fireRayFromPoint(Point start, Point end) {
}

void fizzleRays() {
   long ts = getMsTimestamp();
   for (int i = 0; i < RAY_COUNT; i += 1)
      if (rays[i].spawnTime + RAY_TIMER < ts && rays[i].active) {
         rays[i].active = false;
         hideTube(rays[i].id);
      }
}

Ray *getRays() {
   return rays;
}
