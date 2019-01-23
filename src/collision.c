#include "collision.h"
#include "graphics.h"
#include <stdio.h>
#include <math.h>

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern GLubyte world[WORLDX][WORLDY][WORLDZ];

float boundPoint(const float point, const float dimension) {
   float bounded = point;
   if (-point < 0) bounded = 0.0;
   else if (-point > dimension - 1) bounded = -(dimension - 1);
   return bounded;
}

void boundaryCollision() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   x = boundPoint(x, WORLDX);
   y = boundPoint(y, WORLDY);
   z = boundPoint(z, WORLDZ);
   setViewPosition(x, y, z);
}

void worldCollision() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   int wx = -x, wy = -y, wz = -z;
   if (world[wx][wy][wz] == 1) {
      getOldViewPosition(&x, &y, &z);
      setViewPosition(x, y, z);
   }
}
