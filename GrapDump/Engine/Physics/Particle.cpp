
#include "Particle.hpp"
#include <iostream>

using namespace Physics;

Particle::Particle(double mass = 1.0f) :
    damping(0.9f),
    mass(mass),
    position(Vector3(0.f, 0.f, 0.f)), 
    velocity(Vector3(0.f, 0.f, 0.f)),
    acceleration(Vector3(0.f, 0.f, 0.f)),
    accumulatedForce(Vector3(0.f, 0.f, 0.f)),
    lifespan(rand() % 10 + 1),
    destroyed(false){}

void Particle::UpdatePosition(double deltaTime){
    this->position += 
    this->velocity * deltaTime + 
    this->acceleration * deltaTime * deltaTime / 2.f;
}

void Particle::UpdateVelocity(double deltaTime){
    this->acceleration += this->accumulatedForce * (1 / this->mass);

    this->velocity += this->acceleration * deltaTime;
    this->velocity = this->velocity * powf(this->damping, deltaTime);
}

void Particle::UpdateLifespan(double deltaTime) {
    /* Lifespan deterioration */
    this->lifespan -= deltaTime;
    if (lifespan <= 0) this->Destroy();
}

void Particle::Update(double deltaTime){
    this->UpdatePosition(deltaTime);
    this->UpdateVelocity(deltaTime);
    this->UpdateLifespan(deltaTime);
}

void Particle::Destroy(){
    this->destroyed = true;
}

void Particle::AddForce(Vector3 force){
    this->accumulatedForce += force;
}

void Particle::ResetForce(){
    this->accumulatedForce = Vector3(0.f, 0.f, 0.f);
    this->acceleration = Vector3(0.f, 0.f, 0.f);
}

bool Particle::isDestroyed(){
    return this->destroyed;
}