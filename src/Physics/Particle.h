#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    int radius;
    float mass;

    Particle (float x, float y, float mass);
    ~Particle();

    void Integrate(float dt);
};

#endif
