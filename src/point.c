#include "point.h"
#include <math.h>

bool fequal(float f1, float f2) {
   return fabs(f1 - f2) < 0.1;
}

bool pointsEqual(const Point i, const Point j) {
   return fequal(i.x, j.x) && fequal(i.y, j.y) && fequal(i.z, j.z);
}

bool pointsClose(const Point i, const Point j) {
   int ix = i.x, iy = i.y, iz = i.z;
   int jx = j.x, jy = j.y, jz = j.z;
   return ix == jx && iy == jy && iz == jz;
}

Point vectorBetween(Point start, Point end) {
   return (Point) { end.x - start.x, end.y - start.y, end.z - start.z };
}

Point getEndPoint(Point start, Point next, const float distance) {
   return (Point) {
      start.x - next.x * distance,
      start.y - next.y * distance,
      start.z - next.z * distance
   };
}
