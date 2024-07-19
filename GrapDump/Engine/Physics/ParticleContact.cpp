
#include "ParticleContact.hpp"

using namespace Physics;

void ParticleContact::Resolve(double deltaTime){
    this->ResolveVelocity(deltaTime);

    this->ResolveInterpenetration(deltaTime);
}

void ParticleContact::ResolveInterpenetration(double deltaTime){
    if(this->depth <= 0) return;

    float totalMass = (float) 1 / particles[0]->mass;
    if(particles[1])
        totalMass += (float) 1 / particles[1]->mass;
    
    if(totalMass <= 0) return;

    float totalMoveByMass = this->depth / totalMass;

    Vector3 moveByMass = this->contactNormal * totalMoveByMass;

    Vector3 P_a = moveByMass * ((float) 1 / particles[0]->mass);
    particles[0]->position += P_a;

    if(particles[1]){
        Vector3 P_b = moveByMass * -((float) 1 / particles[1]->mass);
        particles[1]->position += P_b;
    }

    this->depth = 0;
}

float ParticleContact::GetSeparatingSpeed(){
    Vector3 velocity = particles[0]->velocity;

    if(particles[1]) 
        velocity -= particles[1]->velocity;

    return velocity.dot(this->contactNormal);
}

void ParticleContact::ResolveVelocity(double deltaTime){
    float separatingSpeed = this->GetSeparatingSpeed();

    if(separatingSpeed > 0) return;

    float newSS = -this->restitution * separatingSpeed;
    float deltaSpeed = newSS - separatingSpeed;

    float totalMass = (float) 1 / particles[0]->mass;
    if(particles[1])
        totalMass += (float) 1 / particles[1]->mass;

    if(totalMass <= 0) return;

    float impulse_mag = deltaSpeed / totalMass;
    Vector3 Impulse = contactNormal * impulse_mag;

    Vector3 V_a = Impulse * ((float) 1 / particles[0]->mass);
    particles[0]->velocity += V_a;

    if(particles[1]){
        Vector3 V_b = Impulse * -((float) 1 / particles[1]->mass);
        particles[1]->velocity += V_b;
    }
}