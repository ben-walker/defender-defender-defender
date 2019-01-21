#include "collision.h"
#include "graphics.h"
#include <math.h>
#include <stdio.h>

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);

void resetViewPosition() {
   float x, y, z;
   getOldViewPosition(&x, &y, &z);
   setViewPosition(x, y, z);
}

void trueViewPosition(float *x, float *y, float *z) {
   *x = -*x;
   *y = -*y;
   *z = -*z;
}

void catchBoundaryBreak() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   trueViewPosition(&x, &y, &z);
   if (x < 0 || y < 0 || z < 0) resetViewPosition();
   else if (x > WORLDX - 1 || y > WORLDY - 1 || z > WORLDZ - 1) resetViewPosition();
}
