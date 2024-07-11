
#pragma once
#include "../Particle.hpp"
#include "../ParticleContact.hpp"

namespace Physics {
    class ParticleLink {
        public: 
            Particle* particles[2];
            virtual ParticleContact* GetContact() { return nullptr; };

        protected:
            float CurrentLength();
    };
}