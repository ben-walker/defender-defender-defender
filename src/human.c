#include "human.h"
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

   Point legs = { x, y, z, LEGS_COLOR },
      torso = { x, y + 1, z, TORSO_COLOR },
      head = { x, y + 2, z, HEAD_COLOR };

   Human newHuman = { head, torso, legs };
   strncpy(newHuman.name, NAMES[numHumans], NAME_LEN - 1);
   newHuman.name[NAME_LEN - 1] = 0;
   return newHuman;
}

void draw(Human human) {
   int x = human.legs.x, z = human.legs.z;
   world[x][human.legs.y][z] = human.legs.color;
   world[x][human.torso.y][z] = human.torso.color;
   world[x][human.head.y][z] = human.head.color;
}

void erase(Human human) {
   int x = human.legs.x, z = human.legs.z;
   world[x][human.legs.y][z] = 0;
   world[x][human.torso.y][z] = 0;
   world[x][human.head.y][z] = 0;
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

void adjustHumanByVector(Human *human, const int x, const int y, const int z) {
   human->head.x += x; human->torso.x += x; human->legs.x += x;
   human->head.y += y; human->torso.y += y; human->legs.y += y;
   human->head.z += z; human->torso.z += z; human->legs.z += z;
}

void spawnHuman() {
   if (numHumans == MAX_HUMANS)
      return;
   Human human = getNewHuman();
   draw(human);
   trackHumans(human);
}

void applyHumanGravity() {
   for (int i = 0; i < numHumans; i += 1) {
      if (onTheGround(humans[i]))
         continue;
      erase(humans[i]);
      adjustHumanByVector(&humans[i], 0, -1, 0);
      draw(humans[i]);
   }
}

void shootHuman(const int index) {
   printf("You shot %s!\n", humans[index].name);
   erase(humans[index]);
   deleteHumanAt(index);
}

int humanAtPoint(const float fx, const float fy, const float fz) {
   Point point = { fx, fy, fz, 0 };
   int humanIndex = -1;

   for (int i = 0; i < numHumans; i += 1) {
      if (pointsEqual(humans[i].head, point) ||
         pointsEqual(humans[i].torso, point) ||
         pointsEqual(humans[i].legs, point))
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

Human *findNearbyHuman(PointF start, const float maxDist) {
   for (int i = 0; i < numHumans; i += 1) {
      float distance = sqrt(pow(humans[i].head.x - start.x, 2) + pow(humans[i].head.z - start.z, 2));
      if (distance > maxDist)
         continue;
      return &humans[i];
   }
   return NULL;
}
