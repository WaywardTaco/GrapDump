
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <unordered_map>

#include "Shader.hpp"

namespace Engine {
    class Window {
        private:
            GLFWwindow* window;
            bool isOpen;

        public:
            Window(std::string window_name, int window_width, int window_height);
            ~Window();

            void Update();
            
            bool IsOpen();
    };
}