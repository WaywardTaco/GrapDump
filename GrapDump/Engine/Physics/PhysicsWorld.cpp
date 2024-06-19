
#include "PhysicsWorld.hpp"

using namespace Physics;

void PhysicsWorld::AddParticle(Particle* particle){
    this->particles.push_back(particle);

    this->forceRegistry.Add(particle, &this->Gravity);
}

void PhysicsWorld::Update(float deltaTime){
    this->UpdateParticleList();
    this->forceRegistry.UpdateForces(deltaTime);
    for (std::list<Particle*>::iterator itr = particles.begin(); itr != particles.end(); itr++) {
        (*itr)->Update(deltaTime);
    }

}

void PhysicsWorld::UpdateParticleList(){
    particles.remove_if([](Particle* particle){
        return particle->isDestroyed();
    });
}