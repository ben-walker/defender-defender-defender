#ifndef UPDATE_LEVER_
#define UPDATE_LEVER_

#include <stdbool.h>

static long lastUpdateTime = 0;

bool readyToUpdate();

void logUpdateComplete();

#endif // UPDATE_LEVER_
