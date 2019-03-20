#include "vpOps.h"
#include "graphics.h"
#include <math.h>

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);

static const float RAD_CONV = M_PI / 180.0;

Point absViewPos() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return (Point) { fabsf(x), fabsf(y), fabsf(z) };
}

Point nextPos() {
   float xR, yR, zR;
   getViewOrientation(&xR, &yR, &zR);
   return (Point) { sinf(rads(-yR)), sinf(rads(xR)), cosf(rads(-yR)) };
}

float rads(const float deg) {
   return deg * RAD_CONV;
}
