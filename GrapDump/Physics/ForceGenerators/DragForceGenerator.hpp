
#pragma once
#include "../ForceGenerator.hpp"

namespace Physics {
    class DragForceGenerator : public ForceGenerator {
        private:
            double k1 = 0.74f; 
            double k2 = 0.57f;
            // Default is steel to steel drag
        public:
            DragForceGenerator(){};
            DragForceGenerator(double k1, double k2) : k1(k1), k2(k2){};
            void UpdateForce(Particle* particle, double deltaTime) override ;
    };
}