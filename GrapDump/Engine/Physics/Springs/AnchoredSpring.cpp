
#include "AnchoredSpring.hpp"

using namespace Physics;

AnchoredSpring::AnchoredSpring(Vector3 pos, float springConstant, float restLength) :  
    anchorPoint(pos), springConstant(springConstant), restLength(restLength){}

void AnchoredSpring::UpdateForce(Particle* particle, double deltaTime){
    Vector3 pos = particle->position;

    Vector3 force = pos - anchorPoint;
    float mag = force.magnitude();

    float springForce = -springConstant * abs(mag - restLength);

    force = force.normalize();
    force = force * springForce;

    particle->AddForce(force);
}