#include "map.h"
#include "graphics.h"
#include "human.h"
#include "raygun.h"
#include "lander.h"
#include <stdlib.h>
#include <stdio.h>

extern void draw2Dbox(int, int, int, int);
extern void draw2Dline(int, int, int, int, int);
extern void draw2Dtriangle(int, int, int, int, int, int);
extern void set2Dcolour(float []);
extern void getViewPosition(float *, float *, float *);

static GLfloat PLAYER[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat HUMAN[] = { 0.01, 0.18, 0.76, 1.0 };
static GLfloat LANDER[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat SU_LANDER[] = { 0.2, 1.0, 0.0, 1.0 };
static GLfloat BACKGROUND[] = { 1.0, 0.35, 0.0, 0.8 };
static GLfloat FRAME[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat RAY[] = { 1.0, 0.0, 0.83, 1.0 };

static const int HUMAN_SIZE = 2;
static const int LANDER_SIZE = 2;

float mapX(const int x, const float sizeMod) {
   return x + WORLDX * sizeMod;
}

float mapY(const int y, const float sizeMod) {
   return y + WORLDZ * sizeMod;
}

void drawPlayer(const int x, const int y, const float sizeMod) {
   set2Dcolour(PLAYER);
   float playerMod = sizeMod * 2;
   float fx, fy, fz;
   getViewPosition(&fx, &fy, &fz);
   float px = -fx, pz = -fz;
   px = x + px * sizeMod;
   pz = y + pz * sizeMod;
   draw2Dtriangle(px, pz, px - playerMod, pz - playerMod * 3, px + playerMod, pz - playerMod * 3);
}

void drawHumans(const int x, const int y, const float sizeMod) {
   Human *humans = getHumans();
   float bx, by;
   set2Dcolour(HUMAN);

   for (int i = 0; i < currentHumans(); i += 1) {
      if (humans[i].dead)
         continue;
      bx = x + humans[i].legs.x * sizeMod;
      by = y + humans[i].legs.z * sizeMod;
      draw2Dbox(bx, by, bx + sizeMod * HUMAN_SIZE, by + sizeMod * HUMAN_SIZE);
   }
}

void drawLanders(const int x, const int y, const float sizeMod) {
   Lander *landers = getLanders();
   float bx, by, landerMod = sizeMod * LANDER_SIZE;

   for (int i = 0; i < currentLanders(); i += 1) {
      if (landers[i].dead) continue;
      landers[i].super ? set2Dcolour(SU_LANDER) : set2Dcolour(LANDER);
      bx = x + landers[i].center.x * sizeMod;
      by = y + landers[i].center.z * sizeMod;
      draw2Dbox(bx - landerMod, by - landerMod, bx + landerMod, by + landerMod);
   }
}

void drawRays(const int x, const int y, const float sizeMod) {
   Ray *rays = getRays();
   float bx, by, ex, ey;
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

void drawBoundary(const int x, const int y, const float sizeMod) {
   set2Dcolour(BACKGROUND);
   draw2Dbox(x, y, mapX(x, sizeMod), mapY(y, sizeMod));
}

void drawFrame(const int x, const int y, const float sizeMod) {
   set2Dcolour(FRAME);
   draw2Dbox(x - sizeMod, y - sizeMod, mapX(x, sizeMod) + sizeMod, mapY(y, sizeMod) + sizeMod);
}

void drawMap(const int x, const int y, const float sizeMod) {
   drawPlayer(x, y, sizeMod);
   drawHumans(x, y, sizeMod);
   drawLanders(x, y, sizeMod);
   drawRays(x, y, sizeMod);
   drawBoundary(x, y, sizeMod);
   drawFrame(x, y, sizeMod);
}
