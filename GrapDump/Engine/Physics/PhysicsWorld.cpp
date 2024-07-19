
#include "PhysicsWorld.hpp"

using namespace Physics;

PhysicsWorld::PhysicsWorld(Vector3 gravity_vector) :
    Gravity(gravity_vector){}

void PhysicsWorld::GenerateContacts(){
    contacts.clear();

    this->GetOverlaps();

    for(std::list<ParticleLink*>::iterator itr = links.begin(); itr != links.end(); itr++){
        ParticleContact* contact = (*itr)->GetContact();
        if(contact != nullptr)
            contacts.push_back(contact);
    }
}

void PhysicsWorld::GetOverlaps(){
    for(int i = 0 ; i < particles.size() - 1; i++){
        std::list<Particle*>::iterator a = std::next(particles.begin(), i);

        for(int h = i + 1; h < particles.size(); h++){
            std::list<Particle*>::iterator b = std::next(particles.begin(), h);

            Vector3 mag2Vec = (*a)->position - (*b)->position;
            double mag2 = mag2Vec.magnitudeSqrd();

            double rad = (*a)->radius + (*b)->radius;
            double rad2 = rad * rad;

            if(mag2 > rad2) continue;

            Vector3 dir = mag2Vec.normalize();
            double r = rad2 - mag2;
            double depth = sqrt(r);

            double restitution = fmin(
                (*a)->restitution, (*b)->restitution
            );

            AddContact(*a, *b, restitution, dir, depth);
        }
    }
}

void PhysicsWorld::AddParticle(Particle* particle){
    this->particles.push_back(particle);

    if(particle->hasGravity)
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

void PhysicsWorld::AddContact(Particle* p1, Particle* p2, float restitution, Vector3 contactNormal, double depth){
    ParticleContact* toAdd = new ParticleContact();

    toAdd->particles[0] = p1;
    toAdd->particles[1] = p2;
    toAdd->restitution = restitution;
    toAdd->contactNormal = contactNormal;
    toAdd->depth = depth;

    contacts.push_back(toAdd);
}

void PhysicsWorld::UpdateParticleList(){
    /* Particle removal if destroyed */
    particles.remove_if([](Particle* particle){
        return particle->isDestroyed();
    });
}