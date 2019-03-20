#include "vpOps.h"
#include "graphics.h"
#include <math.h>

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);

static const float RAD_CONV = M_PI / 180.0;

void absViewPos(float *x, float *y, float *z) {
   getViewPosition(x, y, z);
   *x = fabsf(*x); *y = fabsf(*y); *z = fabsf(*z);
}

void nextPos(float *x, float *y, float *z) {
   float xrot, yrot, zrot;
   getViewOrientation(&xrot, &yrot, &zrot);
   *x = sinf(rads(-yrot));
   *y = sinf(rads(xrot));
   *z = cosf(rads(-yrot));
}

float rads(const float deg) {
   return deg * RAD_CONV;
}
