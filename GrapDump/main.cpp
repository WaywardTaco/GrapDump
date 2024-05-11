
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "tiny_obj_loader.h"
#endif

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
//#include "PerspectiveCamera.hpp"
#include "OrthoCamera.hpp"
#include "Physics/Vector3.hpp"
//#include "Skybox.hpp"
//#include "LightSource.hpp"
//#include "PointLight.hpp"
//#include "DirectionLight.hpp"
//#include "SpotLight.hpp"
//#include "Player.hpp"

const float
    window_height = 1000,
    window_width = 1000;
const char* 
    window_name = "Josiah Kurt B. Aviso";

GLFWwindow* initializeGLFW();

using namespace Physics;

int main(void)
{
    GLFWwindow* window = initializeGLFW();
    if (window == NULL)
        return -1;

    Shader* mainShader = new Shader("Shader/sample.vert", "Shader/sample.frag");
    //Shader* skyboxShader = new Shader("Shader/Skybox.vert", "Shader/Skybox.frag");

    // See Website for Reference: Anders Riggelsen - Visual glBlendFunc and glBlendEquation Tool
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD); 

    /* Object Declarations */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 1.f));

    Physics::Vector3 pos = Physics::Vector3(-1.f, 0.f, 0.f);

    Model* sphere = new Model("3D/sphere.obj", glm::vec3(0.5f, 0.f, 0.f));
    sphere->setScale(0.3);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        orthoCam->apply(mainShader, NULL);
        
        sphere->setPosition((glm::vec3) pos);
        sphere->render(mainShader);

        pos += Physics::Vector3(0.001, 0.f, 0.f);

        /* Render Frame and Wait for inputs, then resets window & depth buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* initializeGLFW() {
    if (!glfwInit())
        return NULL;

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    /*
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyCallback);
    */

    glEnable(GL_DEPTH_TEST);
    
    return window;
}
