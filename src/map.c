#include "map.h"
#include "graphics.h"
#include "humanHerd.h"
#include "raygun.h"
#include <stdlib.h>

extern void draw2Dbox(int, int, int, int);
extern void draw2Dline(int, int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float []);
extern void getViewPosition(float *, float *, float *);

static GLfloat PLAYER[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat HUMAN[] = {0.01, 0.18, 0.76, 1.0};
static GLfloat BACKGROUND[] = {1.0, 0.35, 0.0, 0.8};
static GLfloat FRAME[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat RAY[] = {1.0, 0.0, 0.83, 1.0};

static const int HUMAN_SIZE = 2;

int mapX(const int x, const int sizeMod) {
   return x + WORLDX * sizeMod;
}

int mapY(const int y, const int sizeMod) {
   return y + WORLDZ * sizeMod;
}

void drawPlayer(const int x, const int y, const int sizeMod) {
   set2Dcolour(PLAYER);
   float fx, fy, fz;
   getViewPosition(&fx, &fy, &fz);
   int px = -fx, pz = -fz;
   px = x + px * sizeMod;
   pz = y + pz * sizeMod;
   draw2Dtriangle(px, pz, px - 5, pz - 15, px + 5, pz - 15);
}

void drawHumans(const int x, const int y, const int sizeMod) {
   Human *humans = getHumans();
   int bx, by;
   set2Dcolour(HUMAN);

   for (int i = 0; i < MAX_HUMANS; i += 1) {
      bx = x + humans[i].legs.x * sizeMod;
      by = y + humans[i].legs.z * sizeMod;
      draw2Dbox(bx, by, bx + sizeMod * HUMAN_SIZE, by + sizeMod * HUMAN_SIZE);
   }
}

void drawRays(const int x, const int y, const int sizeMod) {
   Ray *rays = getRays();
   int bx, by, ex, ey;
   set2Dcolour(RAY);

   for (int i = 0; i < RAY_COUNT; i += 1) {
      if (!rays[i].active)
         continue;
      bx = x + rays[i].start.x * sizeMod;
      by = y + rays[i].start.z * sizeMod;
      ex = x + rays[i].end.x * sizeMod;
      ey = y + rays[i].end.z * sizeMod;
      draw2Dline(bx, by, ex, ey, sizeMod);
   }
}

void drawBoundary(const int x, const int y, const int sizeMod) {
   set2Dcolour(BACKGROUND);
   draw2Dbox(x, y, mapX(x, sizeMod), mapY(y, sizeMod));
}

void drawFrame(const int x, const int y, const int sizeMod) {
   set2Dcolour(FRAME);
   draw2Dbox(x - sizeMod, y - sizeMod, mapX(x, sizeMod) + sizeMod, mapY(y, sizeMod) + sizeMod);
}

void drawMap(const int x, const int y, const int sizeMod) {
   drawPlayer(x, y, sizeMod);
   drawHumans(x, y, sizeMod);
   drawRays(x, y, sizeMod);
   drawBoundary(x, y, sizeMod);
   drawFrame(x, y, sizeMod);
}
