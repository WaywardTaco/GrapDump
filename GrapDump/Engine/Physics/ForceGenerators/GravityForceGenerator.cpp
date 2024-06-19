
#include "GravityForceGenerator.hpp"

using namespace Physics;

void GravityForceGenerator::UpdateForce(Particle* particle, double deltaTime){
    if(particle->mass <= 0) return;

    Vector3 force = this->gravityAcceleration * particle->mass;
    particle->AddForce(force);
}