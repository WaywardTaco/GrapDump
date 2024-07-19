
#pragma once

#include "ParticleLink.hpp"

namespace Physics {
    class AnchoredChain : public ParticleLink {
    public:
        float length = 1;
        float restitution = 0;
        Vector3* anchorPoint = nullptr;

        ParticleContact* GetContact() override;
    };
}