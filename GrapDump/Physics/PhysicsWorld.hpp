
#pragma once

#include <list>
#include "Particle.hpp"
#include "ForceRegistry.hpp"
#include "ForceGenerators/GravityForceGenerator.hpp"

namespace Physics {
    class PhysicsWorld {
        public:
            ForceRegistry forceRegistry;
            std::list<Particle*> particles;
            
        private:
            GravityForceGenerator Gravity = GravityForceGenerator(Vector3(0.f, -9.8f, 0.f));

        public:
            void AddParticle(Particle* particle);
            void Update(float deltaTime);

        private:
            void UpdateParticleList();
    };
}