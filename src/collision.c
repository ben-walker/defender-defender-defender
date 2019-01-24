#include "collision.h"
#include "graphics.h"
#include <stdio.h>
#include <math.h>

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);
extern GLubyte world[WORLDX][WORLDY][WORLDZ];
static const float PREDICTIVE = 1.9;
static const float VIC = 0.4;

bool rawOccupied(const float x, const float y, const float z) {
   return world[(int) -x][(int) -y][(int) -z] != 0;
}

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
   *x = bind(ox, WORLDX); *y = bind(oy, WORLDY); *z = bind(oz, WORLDZ);
}

bool viewPositionOccupied() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return rawOccupied(x, y, z);
}

bool predictiveCollision() {
   float ox, oy, oz, x, y, z;
   getOldViewPosition(&ox, &oy, &oz);
   getViewPosition(&x, &y, &z);
   ox += ((x - ox) * PREDICTIVE); oy += ((y - oy) * PREDICTIVE); oz += ((z - oz) * PREDICTIVE);
   return rawOccupied(ox, oy, oz);
}

bool vicinityOccupied() {
   float x, y, z;
   getViewPosition(&x, &y, &z);
   return rawOccupied(x + VIC, y, z) ||
      rawOccupied(x - VIC, y, z) ||
      rawOccupied(x, y, z + VIC) ||
      rawOccupied(x, y, z - VIC) ||
      rawOccupied(x + VIC, y, z + VIC) ||
      rawOccupied(x + VIC, y, z - VIC) ||
      rawOccupied(x - VIC, y, z + VIC) ||
      rawOccupied(x - VIC, y, z - VIC);
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
   if (viewPositionOccupied() || predictiveCollision() || vicinityOccupied()) {
      resetPosition();
      collision = true;
   }
   return collision;
}

bool collision() {
   return boundaryCollision() || worldCollision();
}
