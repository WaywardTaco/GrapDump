
/*
    This is a chain because it only applies a force to
    stop the attached particle when it would exceed the
    chain's length. Otherwise, the chain applies no force
    to the attached particle and lets it hang freely.
*/

#include "Chain.hpp"

using namespace Physics;

ParticleContact* Chain::GetContact(){
    float currLen = CurrentLength();

    if(currLen <= length) return nullptr;

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = particles[1];
    ret->particles[1] = nullptr;

    Vector3 dir = particles[0]->position - particles[1]->position;
    dir = dir.normalize();

    ret->contactNormal = dir;
    ret->depth = currLen - length;
    ret->restitution = restitution;

    return ret;
}