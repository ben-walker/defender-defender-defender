#include "timeAssistant.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static const int SEC_CONV = 1000;
static const double NSEC_CONV = 1e6;

long getMsTimestamp() {
   struct timespec stamp;
   if (clock_gettime(CLOCK_REALTIME, &stamp) == -1) {
      perror("Could not get time.");
      exit(EXIT_FAILURE);
   }
   return stamp.tv_sec * SEC_CONV + stamp.tv_nsec / NSEC_CONV;
}
