
#pragma once
#include "../ForceGenerator.hpp"

namespace Physics {
    class AnchoredSpring : public ForceGenerator {
        private:
            Vector3 anchorPoint;
            float springConstant;
            float restLength;

        public:
            AnchoredSpring(Vector3 pos, float springConstant, float restLength);

            void UpdateForce(Particle* particle, double deltaTime) override;
    
    };
}

