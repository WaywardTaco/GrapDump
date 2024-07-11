
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <cmath>
#include <cstdlib>
#include "../Vector3.hpp"

namespace Physics {
    using namespace Engine;
    class Particle {
        public:
            double damping;
            double lifespan = 999999999999.f;

            double mass = 1.f;
            double radius = 1.f;
            double restitution = 1.f;

            Vector3 position;
            Vector3 velocity;
            Vector3 acceleration;

            bool hasGravity = true;

        protected:
            bool destroyed;
            Vector3 accumulatedForce;

        public:
            Particle(double mass, bool hasGravity = true);

        protected:
            void UpdatePosition(double deltaTime);
            void UpdateVelocity(double deltaTime);
            void UpdateLifespan(double deltaTime);

        public:
            void Update(double deltaTime);
            void Destroy();

            void AddForce(Vector3 force);
            void ResetForce();

        public:
            bool isDestroyed();
    };
}