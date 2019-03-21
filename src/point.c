#include "point.h"
#include <math.h>

static const float DELTA = 0.1;

bool fequal(float f1, float f2, float delta) {
   return fabs(f1 - f2) < delta;
}

bool pointsEqual(const Point i, const Point j) {
   return fequal(i.x, j.x, DELTA) && fequal(i.y, j.y, DELTA) && fequal(i.z, j.z, DELTA);
}

bool pointsClose(const Point i, const Point j) {
   int ix = i.x, iy = i.y, iz = i.z;
   int jx = j.x, jy = j.y, jz = j.z;
   return ix == jx && iy == jy && iz == jz;
}

bool pointsWithin(const Point i, const Point j, const float delta) {
   return fequal(i.x, j.x, delta) && fequal(i.y, j.y, delta) && fequal(i.z, j.z, delta);
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

Point pointDivision(Point raw, const float value) {
   return (Point) {
      raw.x / value,
      raw.y / value,
      raw.z / value
   };
}

Point addPoints(Point p1, Point p2) {
   return (Point) {
      p1.x + p2.x,
      p1.y + p2.y,
      p1.z + p2.z
   };
}

float vectorMagnitude(Point vec) {
   return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

Point absPoint(Point point) {
   return (Point) {
      fabsf(point.x),
      fabsf(point.y),
      fabsf(point.z)
   };
}

Point unitVector(Point vec) {
   float magnitude = vectorMagnitude(vec);
   return pointDivision(vec, magnitude);
}
