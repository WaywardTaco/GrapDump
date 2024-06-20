
#pragma once

#include "Particle.hpp"
#include "../Model.hpp"
#include "../Shader.hpp"

namespace Physics {
    class RenderParticle {
        private:
            Particle* particle;
            Model* model;
            Vector3* customColor;
            float scale;

        public:
            RenderParticle(Particle* particle, Model* model);
            RenderParticle(Particle* particle, Model* model, Vector3 customColor);
            RenderParticle(Particle* particle, Model* model, Vector3 customColor, float scale);

        public:
            void Update(double deltaTime);
            void Render(Shader* shader);
            Particle* Base();
    };
}