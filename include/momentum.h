#ifndef MOMENTUM_
#define MOMENTUM_

typedef struct Momentum {
   int currentVelocity;
   long lastMovementTS;
} Momentum;

void updateMomentum();

void decayMomentum();

int getCurrentVelocity();

#endif // MOMENTUM_
