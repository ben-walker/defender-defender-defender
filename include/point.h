#ifndef POINT_
#define POINT_

#include <stdbool.h>

typedef struct Point {
   int x, y, z;
   int color;
} Point;

bool pointsEqual(const Point i, const Point j);

#endif // POINT_
