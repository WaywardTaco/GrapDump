
#pragma once

#include "ParticleLink.hpp"

namespace Physics {
    class Chain : public ParticleLink {
    public:
        float length = 1;
        float restitution = 0;

        ParticleContact* GetContact() override;
    };
}
