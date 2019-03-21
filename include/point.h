#ifndef POINT_
#define POINT_

#include <stdbool.h>

typedef struct Point {
   float x, y, z;
} Point;

bool pointsEqual(const Point i, const Point j);

bool pointsClose(const Point i, const Point j);

bool pointsWithin(const Point i, const Point j, const float delta);

Point vectorBetween(Point start, Point end);

Point getEndPoint(Point start, Point next, const float distance);

Point pointDivision(Point raw, const float value);

Point addPoints(Point p1, Point p2);

float vectorMagnitude(Point vec);

Point absPoint(Point point);

Point unitVector(Point vec);

#endif // POINT_
