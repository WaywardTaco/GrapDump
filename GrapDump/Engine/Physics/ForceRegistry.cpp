
#include "ForceRegistry.hpp"

using namespace Physics;

void ForceRegistry::Add(Particle* particle, ForceGenerator* generator){
    /* Adds a particle to the registry with an assosciated generator */
    ParticleForceRegistry pair = ParticleForceRegistry();
    pair.particle = particle;
    pair.generator = generator;

    this->Registry.push_back(pair);
}

void ForceRegistry::Remove(Particle* particle, ForceGenerator* generator){
    /* Clears a particle from a registry as requested */
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
    /* Updates forces applied to all particles within a force registry based on its assosciated generator/s */
    for(std::list<ParticleForceRegistry>::iterator itr = this->Registry.begin(); itr != this->Registry.end(); itr++)
        itr->generator->UpdateForce(itr->particle, deltaTime);
}

