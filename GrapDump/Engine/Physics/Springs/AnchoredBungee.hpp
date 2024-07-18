
#pragma once

#include "../ForceGenerator.hpp"

namespace Physics {
    class AnchoredBungee : public ForceGenerator {
        private:
            Vector3 anchorPoint;
            float springConstant;
            float restLength;

        public:
            AnchoredBungee(Vector3 anchorPoint, float springConstant, float restLength);

            void UpdateForce(Particle* particle, double deltaTime) override;
    
    };
}