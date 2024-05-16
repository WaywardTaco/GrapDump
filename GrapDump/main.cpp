
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
#include <chrono>

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
#include "Physics/Particle.hpp"
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
    window_name = "Josiah Kurt B. Aviso & Dun Gerald C. Baniqued";

GLFWwindow* initializeGLFW();

using namespace std::chrono_literals;
using namespace Physics;

// 16ms as 60 fps translates to 16ms per frame
constexpr std::chrono::nanoseconds timestep(16ms);

int main(void){
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    GLFWwindow* window = initializeGLFW();
    if (window == NULL)
        return -1;

    Shader* mainShader = new Shader("Shader/sample.vert", "Shader/sample.frag");
    //Shader* skyboxShader = new Shader("Shader/Skybox.vert", "Shader/Skybox.frag");

    // See Website for Reference: Anders Riggelsen - Visual glBlendFunc and glBlendEquation Tool
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD); 

    /* Object Declarations */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 1.f));
    orthoCam->setProjection(glm::ortho( - 500.f, 500.f, -500.f, 500.f, 0.1f, 100.f));

    Physics::Vector3 pos = Physics::Vector3(-1.f, 0.f, 0.f);

    Model* sphere = new Model("3D/sphere.obj", glm::vec3(0.5f, 0.f, 0.f));
    sphere->setScale(10.f);

    Particle particle = Particle();
    particle.position = Vector3(0.f, -500.f, 0.f);
    particle.velocity = Vector3(x, y, z);
    particle.acceleration = Vector3(0.f, -50.f, 0.f);
    bool isHitGround = false;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time) ;
        prev_time = curr_time;

        curr_ns += dur;

        if(curr_ns >= timestep){
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;

            particle.Update((double)(ms.count() / 1000.f));
        }

        orthoCam->apply(mainShader, NULL);
        
        sphere->setPosition((glm::vec3) particle.position);
        sphere->render(mainShader);

        //pos += Physics::Vector3(0.001, 0.f, 0.f);

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

    GLFWwindow* window 
        = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return NULL;}

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
