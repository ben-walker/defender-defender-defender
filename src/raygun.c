#include "raygun.h"
#include "timeAssistant.h"
#include "humanHerd.h"
#include <stdio.h>
#include <math.h>

static const float RAD_CONV = M_PI / 180.0;
static const int RAY_COLOR = 6; // pink
static const int RAY_DIST = 150;
static const int RAY_TIMER = 350;
enum RAY_COUNT { RAY_COUNT = 10 };
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

void buildRayUnits(const float bx, const float by, const float bz,
   const float xchange, const float ychange, const float zchange) {
   float ex, ey, ez;
   for (float i = 0.5; i < RAY_DIST; i += 0.5) {
      ex = endPoint(bx, xchange, i);
      ey = endPoint(by, ychange, i);
      ez = endPoint(bz, zchange, i);
      createTube(rayIndex, bx, by, bz, ex, ey, ez, RAY_COLOR);
      humanAtPoint(ex, ey, ez);
   }
}

void spawnRay() {
   float xchange, ychange, zchange, bx, by, bz;
   differentials(&xchange, &ychange, &zchange);
   correctedViewPos(&bx, &by, &bz);
   buildRayUnits(bx, by, bz, xchange, ychange, zchange);
}

void fireRay() {
   rayIndex = (rayIndex == RAY_COUNT - 1) ? 0 : rayIndex + 1;
   Ray newRay = { .id = rayIndex, .spawnTime = getMsTimestamp() };
   rays[rayIndex] = newRay;
   spawnRay();
}

void fizzleRays() {
   long ts = getMsTimestamp();
   for (int i = 0; i < RAY_COUNT; i += 1)
      if (rays[i].spawnTime + RAY_TIMER < ts)
         hideTube(rays[i].id);
}
