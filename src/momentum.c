#include "momentum.h"
#include "timeAssistant.h"
#include <stdlib.h>
#include <time.h>

const float MAX_VELOCITY = 1.0;
const float ACC_RATE = 0.04;
const float DECAY_RATE = 1.5;
const int WAIT_MS = 100;

static Momentum momentum = {0.0, 0};

void fixVelocity() {
   if (momentum.velocity > MAX_VELOCITY)
      momentum.velocity = MAX_VELOCITY;
}

long elapsedTime() {
   return getMsTimestamp() - momentum.lastMovementTS;
}

void updateLastMove() {
   momentum.lastMovementTS = getMsTimestamp();
}

void updateMomentum() {
   if (elapsedTime() < WAIT_MS)
      momentum.velocity += ACC_RATE;
   fixVelocity();
   updateLastMove();
}

void decayMomentum() {
   if (elapsedTime() >= WAIT_MS)
      momentum.velocity /= DECAY_RATE;
}

void killMomentum() {
   momentum.velocity = 0.0;
}

float getVelocity() {
   return momentum.velocity;
}
