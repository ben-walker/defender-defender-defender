#include "humanHerd.h"
#include "graphics.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

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

Human getNewHuman(const int x, const int botY, const int z) {
   Point legs = { x, botY, z, LEGS_COLOR };
   Point torso = { x, botY + 1, z, TORSO_COLOR };
   Point head = { x, botY + 2, z, HEAD_COLOR };
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
   numHumans++;
}

void adjustHumanByVector(Human *human, const int x, const int y, const int z) {
   human->head.x += x; human->torso.x += x; human->legs.x += x;
   human->head.y += y; human->torso.y += y; human->legs.y += y;
   human->head.z += z; human->torso.z += z; human->legs.z += z;
}

void spawnHuman(int x, int y, int z) {
   if (numHumans == MAX_HUMANS)
      return;
   if (occupied(x, y, z))
      y = findNextAvailableY(x, y, z);
   Human human = getNewHuman(x, y, z);
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

void humanAtPoint(const float fx, const float fy, const float fz) {
   Point end = { fx, fy, fz, 0 };
   Human human;
   for (int i = 0; i < numHumans; i += 1) {
      human = humans[i];
      if (pointsEqual(human.head, end) ||
         pointsEqual(human.torso, end) ||
         pointsEqual(human.legs, end))
         printf("You hit %s!\n", human.name);
   }
}

Human *getHumans() {
   return humans;
}
