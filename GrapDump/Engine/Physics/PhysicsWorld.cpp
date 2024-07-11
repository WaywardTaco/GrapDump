
#include "PhysicsWorld.hpp"

using namespace Physics;

void PhysicsWorld::GenerateContacts(){
    contacts.clear();
    for(std::list<ParticleLink*>::iterator itr = links.begin(); itr != links.end(); itr++){
        ParticleContact* contact = (*itr)->GetContact();
        if(contact != nullptr)
            contacts.push_back(contact);
    }
}

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

    this->GenerateContacts();

    if(contacts.size() > 0)
        contactResolver.ResolveContacts(contacts, deltaTime);
}

void PhysicsWorld::ResetForces(){
    /* Resets forces of all particles */
    for (std::list<Particle*>::iterator itr = particles.begin(); itr != particles.end(); itr++) {
        (*itr)->ResetForce();
    }
}

void PhysicsWorld::AddContact(Particle* p1, Particle* p2, float restitution, Vector3 contactNormal){
    ParticleContact* toAdd = new ParticleContact();

    toAdd->particles[0] = p1;
    toAdd->particles[1] = p2;
    toAdd->restitution = restitution;
    toAdd->contactNormal = contactNormal;

    contacts.push_back(toAdd);
}

void PhysicsWorld::UpdateParticleList(){
    /* Particle removal if destroyed */
    particles.remove_if([](Particle* particle){
        return particle->isDestroyed();
    });
}