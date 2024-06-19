
#pragma once
#include "Particle.hpp"

namespace Physics{
    class ForceGenerator {
        public:
            virtual void UpdateForce(Particle* particle, double deltaTime);
    };
}