
#pragma once

#include "RenderParticle.hpp"

namespace Physics {
    class LifespanParticle : public RenderParticle {
        /* A reference class for particles with a lifespan */
        private:
            double _lifespan;

        public:
            LifespanParticle(double lifespan, RenderParticle particle);
            void UpdateLifespan(double lifespanChange);
            double getLifespan();
    };
}