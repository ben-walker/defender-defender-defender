#include "momentum.h"
#include "timeAssistant.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int MAX_VELOCITY = 20;
const int MIN_VELOCITY = 0;
const int VELOCITY_INCREMENT = 2;
const int WAIT_MS = 1000;
static Momentum momentum = {MIN_VELOCITY, 0};

void updateMomentum() {
   long restTime = getMillisecondTimestamp() - momentum.lastMovementTS;
   if (restTime < WAIT_MS && momentum.currentVelocity < MAX_VELOCITY) {
      momentum.currentVelocity += VELOCITY_INCREMENT;
   }
   momentum.lastMovementTS = getMillisecondTimestamp();
}

void decayMomentum() {
   long restTime = getMillisecondTimestamp() - momentum.lastMovementTS;
   if (restTime >= WAIT_MS && momentum.currentVelocity > MIN_VELOCITY) {
      momentum.currentVelocity -= VELOCITY_INCREMENT;
   }
}

int getCurrentVelocity() {
   return momentum.currentVelocity;
}
