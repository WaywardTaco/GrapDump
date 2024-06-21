
#include "ForceGenerator.hpp"

using namespace Physics;

void ForceGenerator::UpdateForce(Particle* particle, double deltaTime){
    /* Simply adds applies the force to a particle */
    particle->AddForce(Vector3(0.f, 0.f, 0.f));
}