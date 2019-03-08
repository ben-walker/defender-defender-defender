#ifndef POINT_
#define POINT_

#include <stdbool.h>

typedef struct Point {
   float x, y, z;
} Point;

bool pointsEqualF(const Point i, const Point j);

#endif // POINT_
