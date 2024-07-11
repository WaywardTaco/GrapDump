
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Vector3.hpp"
#include "GameObject.hpp"

namespace Engine{
    class RenderLine : public GameObject {
        private:
            Vector3 p1, p2, color;
            glm::mat4 projectionMat;

        public:
            RenderLine(Vector3 p1, Vector3 p2, glm::mat4 projection, Vector3 color) :
                p1(p1), p2(p2), color(color), projectionMat(projection) {};

            void Update(double deltaTime);
            void Render(Shader* shader);
    };
}