#ifndef VP_OPS_
#define VP_OPS_

#include "point.h"

Point absViewPos();

Point viewPos();

Point nextPos();

float rads(const float deg);

bool vpIntersect(Point point);

#endif // VP_OPS_
