
#pragma once
#include <list>
#include "Particle.hpp"
#include "ForceGenerator.hpp"

namespace Physics {
    class ForceRegistry {
        public:
            void Add(Particle* particle, ForceGenerator* generator);
            void Remove(Particle* particle, ForceGenerator* generator);
            void Clear();
            void UpdateForces(double deltaTime);   

        protected:
            struct ParticleForceRegistry {
                Particle* particle;
                ForceGenerator* generator;
            };

            std::list<ParticleForceRegistry> Registry;
    };
}