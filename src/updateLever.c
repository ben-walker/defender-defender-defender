#include "updateLever.h"
#include "timeAssistant.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int UPDATE_RESOLUTION_MS = 500;

bool readyToUpdate() {
   long newTime = getMillisecondTimestamp();
   return lastUpdateTime + UPDATE_RESOLUTION_MS <= newTime;
}

void logUpdateComplete() {
   lastUpdateTime = getMillisecondTimestamp();
}
