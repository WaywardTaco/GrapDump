
#include "LifespanParticle.hpp"

using namespace Physics;

LifespanParticle::LifespanParticle(double lifespan, RenderParticle base) :
    RenderParticle(base), _lifespan(lifespan)
{

}

void LifespanParticle::UpdateLifespan(double lifespanChange){
    this->_lifespan += lifespanChange;
}

double LifespanParticle::getLifespan(){
    return this->_lifespan;
}