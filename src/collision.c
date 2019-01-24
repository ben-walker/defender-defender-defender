#include "collision.h"
#include "graphics.h"
#include <stdio.h>
#include <math.h>

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern GLubyte world[WORLDX][WORLDY][WORLDZ];

float bind(const float point, const float dimension) {
   float bounded = point;
   if (-point < 0)
      bounded = 0.0;
   else if (-point > dimension - 1)
      bounded = -(dimension - 1);
   return bounded;
}

void bindViewPosition(float *x, float *y, float *z) {
   float ox, oy, oz;
   getViewPosition(&ox, &oy, &oz);
   *x = bind(ox, WORLDX);
   *y = bind(oy, WORLDY);
   *z = bind(oz, WORLDZ);
}

bool viewPositionOccupied() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return world[(int) -x][(int) -y][(int) -z] != 0;
}

void resetPosition() {
   float x, y, z;
   getOldViewPosition(&x, &y, &z);
   setViewPosition(x, y, z);
}

bool boundaryCollision() {
   float x, y, z, nx, ny, nz;
   getViewPosition(&x, &y, &z);
   bindViewPosition(&nx, &ny, &nz);
   setViewPosition(nx, ny, nz);
   return x != nx || y != ny || z != nz;
}

bool worldCollision() {
   bool collision = false;
   if (viewPositionOccupied()) {
      resetPosition();
      collision = true;
   }
   return collision;
}

bool collision() {
   return boundaryCollision() || worldCollision();
}
