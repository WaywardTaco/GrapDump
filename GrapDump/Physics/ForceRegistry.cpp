
#include "ForceRegistry.hpp"

using namespace Physics;

void ForceRegistry::Add(Particle* particle, ForceGenerator* generator){
    ParticleForceRegistry pair = ParticleForceRegistry();
    pair.particle = particle;
    pair.generator = generator;

    this->Registry.push_back(pair);
}

void ForceRegistry::Remove(Particle* particle, ForceGenerator* generator){
    this->Registry.remove_if(
        [particle, generator](ParticleForceRegistry registry){
            return registry.particle == particle && registry.generator == generator;
        }
    );
}

void ForceRegistry::Clear(){
    this->Registry.clear();
}

void ForceRegistry::UpdateForces(double deltaTime){
    for(std::list<ParticleForceRegistry>::iterator itr = this->Registry.begin(); itr != this->Registry.end(); itr++)
        itr->generator->UpdateForce(itr->particle, deltaTime);
}

