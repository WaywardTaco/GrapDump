
#include "Rod.hpp"

using namespace Physics;

ParticleContact* Rod::GetContact(){
    float currLen = CurrentLength();

    if(currLen == length) return nullptr;

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = particles[0];
    ret->particles[1] = particles[1];

    Vector3 dir = particles[1]->position - particles[0]->position;
    dir = dir.normalize();

    if(currLen > length){
        ret->contactNormal = dir;
        ret->depth = currLen - length;
    } else {
        ret->contactNormal = dir * -1;
        ret->depth = length - currLen;
    }

    ret->restitution = restitution;

    return ret;
}