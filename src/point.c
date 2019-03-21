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

Point pointDivision(Point raw, const int value) {
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
