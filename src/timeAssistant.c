#include "timeAssistant.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

long getMillisecondTimestamp() {
   struct timespec stamp;
   if (clock_gettime(CLOCK_REALTIME, &stamp) == -1) {
      perror("Could not get time.");
      exit(EXIT_FAILURE);
   }
   return stamp.tv_sec * 1000 + stamp.tv_nsec / 1e6;
}
