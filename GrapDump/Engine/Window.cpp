
#include "Window.hpp"

using namespace Engine;

Window::Window(std::string window_name, int window_width, int window_height) :
    isOpen(true)
{
    /* GLFW and window initializations */
    this->window = NULL;
    
    if (!glfwInit()) {
        std::cout << "[ERROR] GLFW failed to initialize" << std::endl;
        return;
    }

    this->window = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);

    if (!this->window) {
        std::cout << "[ERROR] Window \"" << window_name << "\" (" << window_width << "x" << window_height << ") failed to initialize" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(this->window);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
}

Window::~Window(){
    
}

void Window::Update(){
    /* Stops rendering to a closed window */
    if(!this->IsOpen()) return;

    /* Sets buffer to window frame and clears it for the next render update */
    glfwSwapBuffers(this->window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::IsOpen(){
    if(glfwWindowShouldClose(this->window) || this->window == NULL)
        this->isOpen = false;
    return this->isOpen;
}

GLFWwindow* Window::Base(){
    return this->window;
}