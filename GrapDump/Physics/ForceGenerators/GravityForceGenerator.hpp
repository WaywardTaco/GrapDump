
#pragma once
#include "../ForceGenerator.hpp"

namespace Physics {
    class GravityForceGenerator : public ForceGenerator {
        private:
            Vector3 gravityAcceleration = Vector3(0.f, -9.8f, 0.f);
        public:
            GravityForceGenerator(const Vector3 gravity) : gravityAcceleration(gravity){};
            void UpdateForce(Particle* particle, double deltaTime) override ;
    };
}