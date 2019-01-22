#include "updateLever.h"
#include "timeAssistant.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int UPDATE_RESOLUTION_MS = 50;
static long lastUpdateTime = 0;

bool readyToUpdate() {
   long newTime = getMsTimestamp();
   return lastUpdateTime + UPDATE_RESOLUTION_MS <= newTime;
}

void logUpdateComplete() {
   lastUpdateTime = getMsTimestamp();
}
