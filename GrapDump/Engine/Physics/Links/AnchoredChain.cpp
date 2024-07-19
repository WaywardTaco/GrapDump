
#include "AnchoredChain.hpp"

using namespace Physics;

ParticleContact* AnchoredChain::GetContact(){
    float currLen = this->anchorPoint->distanceFrom(this->particles[0]->position);

    if(currLen <= length) return nullptr;

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = particles[0];
    ret->particles[1] = nullptr;

    Vector3 dir = *(this->anchorPoint) - particles[0]->position;
    dir = dir.normalize();

    ret->contactNormal = dir;
    ret->depth = currLen - length;
    ret->restitution = restitution;

    return ret;
}