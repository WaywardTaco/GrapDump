
#include "PhysicsWorld.hpp"

using namespace Physics;

void PhysicsWorld::AddParticle(Particle* particle){
    this->particles.push_back(particle);

    this->forceRegistry.Add(particle, &this->Gravity);
}

void PhysicsWorld::Update(float deltaTime){
    /* Removes all destroyed particles from the list */
    this->UpdateParticleList();
    /* Updates forces of its own registry */
    this->forceRegistry.UpdateForces(deltaTime);
    /* Updates the forces, acceleration, velocity, and positions of all particles */
    for (std::list<Particle*>::iterator itr = particles.begin(); itr != particles.end(); itr++) {
        (*itr)->Update(deltaTime);
    }
}

void PhysicsWorld::ResetForces(){
    /* Resets forces of all particles */
    for (std::list<Particle*>::iterator itr = particles.begin(); itr != particles.end(); itr++) {
        (*itr)->ResetForce();
    }
}

void PhysicsWorld::UpdateParticleList(){
    /* Particle removal if destroyed */
    particles.remove_if([](Particle* particle){
        return particle->isDestroyed();
    });
}