
#include "ForceGenerator.hpp"

using namespace Physics;

void ForceGenerator::UpdateForce(Particle* particle, double deltaTime){
    particle->AddForce(Vector3(0.f, 0.f, 0.f));
}