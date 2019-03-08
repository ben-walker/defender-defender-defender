#include "human.h"
#include "lander.h"
#include "graphics.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern GLubyte world[WORLDX][WORLDY][WORLDZ];
static const int HEAD_COLOR = 1;
static const int TORSO_COLOR = 3;
static const int LEGS_COLOR = 7;

static Human humans[MAX_HUMANS];
static int numHumans = 0;
static const float fatalHeight = 10.0;

bool occupied(int x, int y, int z) {
   return world[x][y][z] != 0;
}

int findNextAvailableY(const int x, int y, const int z) {
   while (occupied(x, y, z))
      y += 1;
   return y;
}

Human getNewHuman() {
   int x = rand() % WORLDX, y = 0, z = rand() % WORLDZ;
   if (occupied(x, y, z))
      y = findNextAvailableY(x, y, z);

   Human newHuman = {
      .head = { x, y + 2, z },
      .torso = { x, y + 1, z },
      .legs = { x, y, z },
      .name = NAMES[numHumans],
      .captive = false,
      .fallStart = -1.0
   };
   return newHuman;
}

void draw(Human human) {
   int x = human.legs.x, z = human.legs.z;
   world[x][(int) human.legs.y][z] = LEGS_COLOR;
   world[x][(int) human.torso.y][z] = TORSO_COLOR;
   world[x][(int) human.head.y][z] = HEAD_COLOR;
}

void erase(Human human) {
   int x = human.legs.x, z = human.legs.z;
   world[x][(int) human.legs.y][z] = 0;
   world[x][(int) human.torso.y][z] = 0;
   world[x][(int) human.head.y][z] = 0;
}

bool onTheGround(Human human) {
   int x = human.legs.x, z = human.legs.z;
   return occupied(x, human.legs.y - 1, z);
}

void trackHumans(Human human) {
   humans[numHumans] = human;
   numHumans += 1;
}

void deleteHumanAt(int index) {
   for (; index < numHumans - 1; index += 1)
      humans[index] = humans[index + 1];
   numHumans -= 1;
}

void deleteHumanByName(const char *name) {
   int i;
   for (i = 0; i < numHumans; i += 1) {
      if (strcmp(name, humans[i].name) == 0)
         break;
   }
   if (i == numHumans)
      return;
   deleteHumanAt(i);
}

void adjustHumanByVector(Human *human, Point vector) {
   erase(*human);
   human->head.x += vector.x; human->torso.x += vector.x; human->legs.x += vector.x;
   human->head.y += vector.y; human->torso.y += vector.y; human->legs.y += vector.y;
   human->head.z += vector.z; human->torso.z += vector.z; human->legs.z += vector.z;
   draw(*human);
}

void spawnHuman() {
   if (numHumans == MAX_HUMANS)
      return;
   Human human = getNewHuman();
   draw(human);
   trackHumans(human);
}

void calculateFall(Human *human) {
   if (human->fallStart == -1.0)
      return;
   float fallDistance = human->fallStart - human->legs.y;
   human->fallStart = -1.0;
   if (fallDistance < fatalHeight)
      return;
   printf("%s perished after an unfortunate fall!\n", human->name);
   erase(*human);
   deleteHumanByName(human->name);
}

void applyHumanGravity() {
   for (int i = 0; i < numHumans; i += 1) {
      if (humans[i].captive)
         continue;
      else if (onTheGround(humans[i])) {
         calculateFall(&humans[i]);
         continue;
      }
      if (humans[i].fallStart == -1.0)
         humans[i].fallStart = humans[i].legs.y;
      adjustHumanByVector(&humans[i], (Point) { 0, -1, 0 });
   }
}

void shootHuman(const int index) {
   printf("You shot %s!\n", humans[index].name);
   erase(humans[index]);
   if (humans[index].captive)
      signalLostCaptive(humans[index].name);
   deleteHumanAt(index);
}

int humanAtPoint(const float fx, const float fy, const float fz) {
   Point point = { fx, fy, fz };
   int humanIndex = -1;

   for (int i = 0; i < numHumans; i += 1) {
      if (pointsClose(humans[i].head, point) ||
         pointsClose(humans[i].torso, point) ||
         pointsClose(humans[i].legs, point))
            humanIndex = i;
   }
   return humanIndex;
}

Human *getHumans() {
   return humans;
}

int currentHumans() {
   return numHumans;
}

Human *findNearbyHuman(Point start, const float maxDist) {
   for (int i = 0; i < numHumans; i += 1) {
      float distance = sqrt(pow(humans[i].head.x - start.x, 2) + pow(humans[i].head.z - start.z, 2));
      if (distance > maxDist)
         continue;
      return &humans[i];
   }
   return NULL;
}

void markCaptive(const char *name) {
   for (int i = 0; i < numHumans; i += 1) {
      if (strcmp(name, humans[i].name) == 0) {
         humans[i].captive = true;
         break;
      }
   }
}
