#include "raygun.h"
#include "timeAssistant.h"
#include <stdio.h>
#include <math.h>

static const float HALF_CIRCLE = 180.0;
static const int RAY_COLOR = 6; // pink
static const int RAY_DIST = 150;
enum RAY_COUNT { RAY_COUNT = 10 };
static Ray rays[RAY_COUNT];
static int rayIndex = -1;

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);

float rads(const float deg) {
   return deg * (M_PI / HALF_CIRCLE);
}

float endRayPos(const float pointStart, const float pointChange) {
   return pointStart - pointChange * RAY_DIST;
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

void spawnRay(const int index) {
   float xchange, ychange, zchange, bx, by, bz;
   differentials(&xchange, &ychange, &zchange);
   correctedViewPos(&bx, &by, &bz);
   float ex = endRayPos(bx, xchange),
      ey = endRayPos(by, ychange),
      ez = endRayPos(bz, zchange);
   createTube(index, bx, by, bz, ex, ey, ez, RAY_COLOR);
}

void fireRay() {
   rayIndex = (rayIndex == RAY_COUNT - 1) ? 0 : rayIndex + 1;
   Ray newRay = { .id = rayIndex, .spawnTime = getMsTimestamp() };
   rays[rayIndex] = newRay;
   spawnRay(newRay.id);
}

void fizzleRays() {
   long ts = getMsTimestamp();
   for (int i = 0; i < RAY_COUNT; i += 1)
      if (rays[i].spawnTime + 350 < ts)
         hideTube(rays[i].id);
}
