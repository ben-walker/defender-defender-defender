#include "raygun.h"
#include "timeAssistant.h"
#include "human.h"
#include "lander.h"
#include "vpOps.h"
#include <stdio.h>
#include <math.h>

static const int RAY_COLOR = 6, // pink
   RAY_DIST = 150,
   RAY_LIFESPAN = 350; // ms
static const float RAY_STEP = 0.5;
static Ray rays[RAY_COUNT]; // 10 rays
static int rayIndex = 0;

extern void getViewOrientation(float *, float *, float *);
extern void getViewPosition(float *, float *, float *);
extern void createTube(int, float, float, float, float, float, float, int);
extern void hideTube(int);

bool rayInvalid(Ray ray) {
   return (ray.spawnTime + RAY_LIFESPAN) < getMsTimestamp() && ray.active;
}

int nextRay() {
   return (rayIndex == RAY_COUNT - 1) ? 0 : rayIndex + 1;
}

Ray getNewRay(Point start, Point unitVector) {
   Ray newRay = {
      .id = rayIndex,
      .spawnTime = getMsTimestamp(),
      .active = true,
      .start = start,
      .unitVector = unitVector
   };
   return newRay;
}

void trackRay(Ray ray) {
   rays[rayIndex] = ray;
   rayIndex = nextRay();
}

void spawnTube(Ray ray, Point point) {
   createTube(ray.id, ray.start.x, ray.start.y, ray.start.z, point.x, point.y, point.z, RAY_COLOR);
}

void checkForHit(Point point) {
   int humanIndex, landerIndex;
   if ((humanIndex = humanAtPoint(point)) != -1)
      shootHuman(humanIndex);
   if ((landerIndex = landerAtPoint(point)) != -1)
      shootLander(landerIndex);
}

void drawRay(Ray *ray) {
   Point step;
   for (float i = RAY_STEP; i < RAY_DIST; i += RAY_STEP) {
      step = getEndPoint(ray->start, ray->unitVector, i);
      spawnTube(*ray, step);
      checkForHit(step);
   }
   ray->end = step;
}

void fireRayFromVP() {
   Ray ray = getNewRay(absViewPos(), nextPos());
   drawRay(&ray);
   trackRay(ray);
}

void fireRayFromPoint(Point start, Point end) {
}

void disableRay(Ray *ray) {
   ray->active = false;
   hideTube(ray->id);
}

void fizzleRays() {
   for (int i = 0; i < RAY_COUNT; i += 1)
      if (rayInvalid(rays[i])) disableRay(&rays[i]);
}

Ray *getRays() {
   return rays;
}
