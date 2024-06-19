
#include "DragForceGenerator.hpp"

using namespace Physics;

void DragForceGenerator::UpdateForce(Particle* particle, double deltaTime){
    Vector3 force = Vector3(0.f, 0.f, 0.f);
    Vector3 currV = particle->velocity;

    double mag = currV.magnitude();
    if(mag <= 0) return;

    double dragF = (k1 * mag) + (k2 * mag);
    Vector3 dir = currV.normalize();

    particle->AddForce(dir * -dragF);
}