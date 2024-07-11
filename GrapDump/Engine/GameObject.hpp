
#pragma once

#include "Shader.hpp"

namespace Engine {
    class GameObject {
        public:
            virtual void Update(double deltaTime) = 0;
            virtual void Render(Shader* shader) = 0;
    };
}