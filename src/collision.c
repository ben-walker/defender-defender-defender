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
   if (-point < 0) bounded = 0.0;
   else if (-point > dimension - 1) bounded = -(dimension - 1);
   return bounded;
}

void resetPosition() {
   float x, y, z;
   getOldViewPosition(&x, &y, &z);
   setViewPosition(x, y, z);
}

bool boundaryCollision() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   float nx = bind(x, WORLDX), ny = bind(y, WORLDY), nz = bind(z, WORLDZ);
   setViewPosition(nx, ny, nz);
   return x != nx || y != ny || z != nz;
}

bool worldCollision() {
   bool collision = false;
   float x, y, z;
   getViewPosition(&x, &y, &z);
   if (world[(int) -x][(int) -y][(int) -z] == 1) {
      resetPosition();
      collision = true;
   }
   return collision;
}

bool collision() {
   return boundaryCollision() || worldCollision();
}
