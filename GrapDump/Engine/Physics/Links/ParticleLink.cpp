
#include "ParticleLink.hpp"

using namespace Physics;

float ParticleLink::CurrentLength(){
    return this->particles[0]->position.distanceFrom(this->particles[1]->position);
}