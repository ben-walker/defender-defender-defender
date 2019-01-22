#include "momentousMovement.h"
#include "graphics.h"

extern void setViewPosition(float, float, float);
extern void getViewPosition(float *, float *, float *);
extern void getOldViewPosition(float *, float *, float *);

float calcMovement(const float newCoord, const float oldCoord, const float velocity) {
   return (newCoord - oldCoord) * velocity;
}

void moveWithMomentum(const float velocity) {
   float ox, oy, oz, x, y, z;
   getOldViewPosition(&ox, &oy, &oz);
   getViewPosition(&x, &y, &z);

   float xmod = calcMovement(x, ox, velocity);
   float ymod = calcMovement(y, oy, velocity);
   float zmod = calcMovement(z, oz, velocity);

   setViewPosition(x + xmod, y + ymod, z + zmod);
}
