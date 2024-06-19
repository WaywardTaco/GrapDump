
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <iomanip>

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

#include "Engine/Shader.hpp"
#include "Engine/Model.hpp"
#include "Engine/Cameras/Camera.hpp"
//#include "PerspectiveCamera.hpp"
#include "Engine/Cameras/OrthoCamera.hpp"
#include "Engine/Physics/Vector3.hpp"
#include "Engine/Physics/Particle.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"
#include "Engine/Physics/RenderParticle.hpp"
#include "Engine/Physics/ForceGenerators//DragForceGenerator.hpp"
//#include "Skybox.hpp"
//#include "LightSource.hpp"
//#include "PointLight.hpp"
//#include "DirectionLight.hpp"
//#include "SpotLight.hpp"
//#include "Player.hpp"

const float
    window_height = 700,
    window_width = 700;
const char* 
    window_name = "Josiah Kurt B. Aviso";

GLFWwindow* initializeGLFW();

using namespace std::chrono_literals;
using namespace Physics;

// 16ms as 60 fps translates to 16ms per frame
constexpr std::chrono::nanoseconds timestep(16ms);

int main(void){

    GLFWwindow* window = initializeGLFW();
    if (window == NULL)
        return -1;

    Shader* mainShader = new Shader("Shader/sample.vert", "Shader/sample.frag");
    PhysicsWorld world = PhysicsWorld();
    std::list<RenderParticle*> renderedParticles;

    /* Object Declarations */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 350.f));
    orthoCam->setProjection(glm::ortho( - 350.f, 350.f, -350.f, 350.f, 0.1f, 700.f));

    Model* particleSphere = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));
    particleSphere->setScale(15.f);

    Particle redParticle = Particle(1.0f);
    redParticle.position = Vector3(0, 350, 0);
    world.AddParticle(&redParticle);
    renderedParticles.push_back(new RenderParticle(&redParticle, particleSphere, Vector3(0.6f, 0.f, 0.f)));


    Particle blueParticle = Particle(1.0f);
    blueParticle.position = Vector3(0, 350, 0);
    world.AddParticle(&blueParticle);
    renderedParticles.push_back(new RenderParticle(&blueParticle, particleSphere, Vector3(0.f, 0.f, 0.6f)));

    // Wood on Ice
    DragForceGenerator drag = DragForceGenerator(0.14f, 0.1f);
    world.forceRegistry.Add(&redParticle, &drag);


    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    auto start_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

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
            
            world.Update(ms.count() / 1000.f);
        }

        orthoCam->apply(mainShader, NULL);

        for (RenderParticle* particle : renderedParticles) 
            particle->Render(mainShader);

        redParticle.ResetForce();
        blueParticle.ResetForce();

        redParticle.AddForce(Vector3(5, 0, 0));
        blueParticle.AddForce(Vector3(5, 0, 0));

        /* End of frame clear buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

void Update() {

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

    glEnable(GL_DEPTH_TEST);
    
    return window;
}