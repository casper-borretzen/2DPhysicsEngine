#include "Body.h"
#include <math.h>
#include <iostream>
#include "../Graphics.h"

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;
    this->friction = 0.7;
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    this->I = shape.GetMomentOfInertia() * mass;
    if (this->I != 0.0) {
        this->invI = 1.0 / this->I;
    } else {
        this->invI = 0.0;
    }
    std::cout << "Body contructor called!" << std::endl;
}

Body::~Body() {
    delete shape;
    SDL_DestroyTexture(texture);
    std::cout << "Body destructor called!" << std::endl;
}

void Body::SetTexture(const char* textureFileName) {
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

bool Body::IsStatic() const {
    const float epsilon = 0.005;
    return fabs(invMass - 0.0) < epsilon;
}

void Body::ApplyImpulse(const Vec2& j) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2& j, const Vec2& r) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::ClearTorque() {
    sumTorque = 0.0;
}

void Body::IntegrateLinear(float dt) {
    if (IsStatic()) {
        return;
    }
    acceleration = sumForces * invMass;
    velocity += acceleration * dt;
    position += velocity * dt;
    ClearForces();
}

void Body::IntegrateAngular(float dt) {
    if (IsStatic()) {
        return;
    }
    angularAcceleration = sumTorque * invI;
    angularVelocity += angularAcceleration * dt;
    rotation += angularVelocity * dt;
    ClearTorque();
}

void Body::Update(float dt) {
        IntegrateLinear(dt);
        IntegrateAngular(dt);
        shape->UpdateVertices(rotation, position);
}
