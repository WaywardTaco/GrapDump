
#pragma once

#include <list>
#include "Particle.hpp"
#include "ForceRegistry.hpp"
#include "ForceGenerators/GravityForceGenerator.hpp"
#include "ParticleContact.hpp"
#include "ContactResolver.hpp"
#include "Links/ParticleLink.hpp"
#include "Springs/AnchoredSpring.hpp"
#include "Links/Rod.hpp"

namespace Physics {
    class PhysicsWorld {
        public:
            ForceRegistry forceRegistry;
            std::list<Particle*> particles;
            std::vector<ParticleContact*> contacts;
            std::list<ParticleLink*> links;

        protected:
            ContactResolver contactResolver = ContactResolver(20);
            void GenerateContacts();
            void GetOverlaps();

        private:
            GravityForceGenerator Gravity;
            
        public:
            PhysicsWorld(Vector3 gravity_vector = Vector3(0.f, -9.8f, 0.f));

            void AddParticle(Particle* particle);
            void Update(float deltaTime);
            void ResetForces();
            void AddContact(Particle* p1, Particle* p2, float restitution, Vector3 contactNormal);
            void AddContact(Particle* p1, Particle* p2, float restitution, Vector3 contactNormal, double depth);

        private:
            void UpdateParticleList();
    };
}