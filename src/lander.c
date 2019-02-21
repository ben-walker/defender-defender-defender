#include "lander.h"
#include "graphics.h"

extern GLubyte world[WORLDX][WORLDY][WORLDZ];

static Lander landers[MAX_LANDERS];
static int numLanders = 0;

void addLander() {
   if (numLanders == MAX_LANDERS)
      return;
}
