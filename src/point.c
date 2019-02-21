#include "point.h"

bool pointsEqual(const Point i, const Point j) {
   return i.x == j.x &&
      i.y == j.y &&
      i.z == j.z;
}
