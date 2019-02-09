#include "map.h"
#include "graphics.h"

extern void draw2Dbox(int, int, int, int);
extern void set2Dcolour(float []);

void drawBoundary(const int x, const int y, const int sizeMod) {
   GLfloat backdrop[] = {1.0, 0.35, 0.0, 0.8};
   set2Dcolour(backdrop);
   draw2Dbox(x, y, x + WORLDX * sizeMod, y + WORLDZ * sizeMod);
}

void drawMap(const int x, const int y, const int sizeMod) {
   drawBoundary(x, y, sizeMod);
}
