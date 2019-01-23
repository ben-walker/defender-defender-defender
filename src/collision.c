#include "collision.h"
#include "graphics.h"

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);

float boundPoint(const float point, const float dimension) {
   float bounded = point;
   if (-point < 0) bounded = 0.0;
   else if (-point > dimension - 1) bounded = -(dimension - 1);
   return bounded;
}

void catchBoundaryBreak() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   x = boundPoint(x, WORLDX);
   y = boundPoint(y, WORLDY);
   z = boundPoint(z, WORLDZ);
   setViewPosition(x, y, z);
}
