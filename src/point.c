#include "point.h"
#include <math.h>

bool fequal(float f1, float f2) {
   return fabs(f1 - f2) < 0.1;
}

bool pointsEqualF(const Point i, const Point j) {
   return fequal(i.x, j.x) && fequal(i.y, j.y) && fequal(i.z, j.z);
}
