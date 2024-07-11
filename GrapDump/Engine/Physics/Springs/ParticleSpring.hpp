
#pragma once
#include "../Particle.hpp"
#include "../ForceGenerator.hpp"

namespace Physics {
    class ParticleSpring : public ForceGenerator {
        private:
            Particle* anchorParticle;
            float springConstant;
            float restLength;

        public:
            ParticleSpring(Particle* anchorParticle, float springConstant, float restLength);

            void UpdateForce(Particle* particle, double deltaTime) override;
    
    };
}