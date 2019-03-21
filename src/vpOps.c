#include "vpOps.h"
#include "graphics.h"
#include <math.h>
#include <stdbool.h>

static const float VP_BUFFER = 0.5;

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);

static const float RAD_CONV = M_PI / 180.0;

Point absViewPos() {
   return absPoint(viewPos());
}

Point viewPos() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return (Point) { x, y, z };
}

Point nextPos() {
   float xR, yR, zR;
   getViewOrientation(&xR, &yR, &zR);
   return (Point) { sinf(rads(-yR)), sinf(rads(xR)), cosf(rads(-yR)) };
}

float rads(const float deg) {
   return deg * RAD_CONV;
}

bool vpIntersect(Point point) {
   return pointsWithin(point, absViewPos(), VP_BUFFER);
}
