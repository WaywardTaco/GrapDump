
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <cmath>
#include "Vector3.hpp"

namespace Physics {
    class Particle {
        public:
            double damping;

            double mass;
            Vector3 position;
            Vector3 velocity;
            Vector3 acceleration;

        protected:
            bool destroyed;
            Vector3 accumulatedForce;

        public:
            Particle(double mass);

        protected:
            void UpdatePosition(double deltaTime);
            void UpdateVelocity(double deltaTime);

        public:
            void Update(double deltaTime);
            void Destroy();

            void AddForce(Vector3 force);
            void ResetForce();

        public:
            bool isDestroyed();
    };
}