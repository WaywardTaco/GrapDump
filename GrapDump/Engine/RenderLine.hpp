
#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Vector3.hpp"
#include "Cameras/Camera.hpp"

namespace Engine{
    class RenderLine {
        private:
            Vector3 *p1, *p2, color;
            

        public:
            RenderLine(Vector3* p1, Vector3* p2, Vector3 color) :
                p1(p1), p2(p2), color(color) {};

            void Update(double deltaTime);
            void Render(Shader* shader, Camera* camera, bool isOrtho);
    };
}