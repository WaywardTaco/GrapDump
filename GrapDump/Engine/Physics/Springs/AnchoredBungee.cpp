
/*
    This is a Bungee because it only applies
    its spring force when the object stretches
    the Bungee beyond its length, pulling the
    attached particle back towards the Bungee's
    anchor point up until the Bungee is no
    longer stretched beyond its length.
*/

#include "AnchoredBungee.hpp"

using namespace Physics;

AnchoredBungee::AnchoredBungee(Vector3 anchorPoint, float springConstant, float restLength) :  
    anchorPoint(anchorPoint), springConstant(springConstant), restLength(restLength){}

void AnchoredBungee::UpdateForce(Particle* particle, double deltaTime){
    Vector3 pos = particle->position;

    Vector3 force = pos - anchorPoint;
    float mag = force.magnitude();

    if(mag <= restLength) return;

    float springForce = -springConstant * abs(mag - restLength);

    force = force.normalize();
    force = force * springForce;
    
    particle->AddForce(force);
}