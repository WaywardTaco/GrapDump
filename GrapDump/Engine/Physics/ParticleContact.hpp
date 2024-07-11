
#pragma once

#include "Particle.hpp"

namespace Physics {
    class ParticleContact{
        public:
            float depth;

            Particle* particles[2];

            float restitution;

            Vector3 contactNormal;

        public:
            ParticleContact() : contactNormal(0, 0, 0){};
            void Resolve(double deltaTime);
            void ResolveInterpenetration(double deltaTime);
            float GetSeparatingSpeed();

        protected:
            void ResolveVelocity(double deltaTime);
    };
}