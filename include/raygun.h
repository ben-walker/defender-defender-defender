#ifndef RAYGUN_
#define RAYGUN_

#include "point.h"
#include <stdbool.h>

enum RAY_COUNT { RAY_COUNT = 10 };

typedef struct Ray {
   int id;
   long spawnTime;
   bool active;
   Point start, end;
} Ray;

void fireRayFromVP();

void fizzleRays();

Ray *getRays();

#endif // RAYGUN_
