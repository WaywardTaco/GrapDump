
#include "ParticleSpring.hpp"

using namespace Physics;

ParticleSpring::ParticleSpring(Particle* anchorParticle, float springConstant, float restLength) :  
    anchorParticle(anchorParticle), springConstant(springConstant), restLength(restLength){}

void ParticleSpring::UpdateForce(Particle* particle, double deltaTime){
    Vector3 pos = particle->position;

    Vector3 force = pos - this->anchorParticle->position;
    float mag = force.magnitude();

    float springForce = -springConstant * abs(mag - restLength);

    force = force.normalize();
    force = force * springForce;
}