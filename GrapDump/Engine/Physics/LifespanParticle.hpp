
#pragma once

#include <RenderParticle.hpp>

namespace Physics {
    class LifespanParticle : public RenderParticle {
        private:
            double _lifespan;

        public:
            LifespanParticle(double lifespan, RenderParticle particle);
            void UpdateLifespan(double lifespanChange);
            double getLifespan();
    };
}