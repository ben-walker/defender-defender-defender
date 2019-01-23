#ifndef MOMENTUM_
#define MOMENTUM_

typedef struct Momentum {
   float velocity;
   long lastMovementTS;
} Momentum;

void updateMomentum();

void decayMomentum();

void killMomentum();

float getVelocity();

#endif // MOMENTUM_
