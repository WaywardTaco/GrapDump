
#include "Particle.hpp"
#include <iostream>

using namespace Physics;

Particle::Particle(double mass) :
    mass(mass),
    position(Vector3(0.f, 0.f, 0.f)), 
    velocity(Vector3(0.f, 0.f, 0.f)),
    acceleration(Vector3(0.f, 0.f, 0.f)){}

void Particle::UpdatePosition(double deltaTime){
    this->position += 
        this->velocity * deltaTime + 
        this->acceleration * deltaTime * deltaTime / 2.f;
}

void Particle::UpdateVelocity(double deltaTime){
    this->velocity += this->acceleration * deltaTime;
}

void Particle::Update(double deltaTime){
    this->UpdatePosition(deltaTime);
    this->UpdateVelocity(deltaTime);
}