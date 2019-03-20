#ifndef POINT_
#define POINT_

#include <stdbool.h>

typedef struct Point {
   float x, y, z;
} Point;

bool pointsEqual(const Point i, const Point j);

bool pointsClose(const Point i, const Point j);

Point vectorBetween(Point start, Point end);

#endif // POINT_
