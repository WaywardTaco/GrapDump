
#include "Particle.hpp"

using namespace Physics;

Particle::Particle(double mass = 0) :
    mass(mass),
    position(Vector3(0.f, 0.f, 0.f)), 
    velocity(Vector3(0.f, 0.f, 0.f)),
    acceleration(Vector3(0.f, 0.f, 0.f)){}

void Particle::UpdatePosition(double deltaTime){
    
}

void Particle::UpdateVelocity(double deltaTime){

}

void Particle::Update(double deltaTime){

}