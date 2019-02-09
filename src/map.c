#include "map.h"
#include "graphics.h"
#include "humanHerd.h"

extern void draw2Dbox(int, int, int, int);
extern void set2Dcolour(float []);

static GLfloat HUMAN_COLOR[] = {0.16, 1.0, 0.04, 1.0};
static GLfloat BACK_COLOR[] = {1.0, 0.35, 0.0, 0.8};

void drawBoundary(const int x, const int y, const int sizeMod) {
   set2Dcolour(BACK_COLOR);
   draw2Dbox(x, y, x + WORLDX * sizeMod, y + WORLDZ * sizeMod);
}

void drawHumans(const int x, const int y, const int sizeMod) {
   Human *humans = getHumans();
   int bx, by;
   set2Dcolour(HUMAN_COLOR);

   for (int i = 0; i < MAX_HUMANS; i += 1) {
      bx = x + humans[i].legs.x * sizeMod;
      by = y + humans[i].legs.z * sizeMod;
      draw2Dbox(bx, by, bx + sizeMod, by + sizeMod);
   }
}

void drawMap(const int x, const int y, const int sizeMod) {
   drawHumans(x, y, sizeMod);
   drawBoundary(x, y, sizeMod);
}
