
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
#include <cstdlib>
#include <iostream>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "tiny_obj_loader.h"
#endif

#include "Engine/Window.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Model.hpp"
#include "Engine/Cameras/Camera.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Cameras/OrthoCamera.hpp"
#include "Engine/Cameras/PerspectiveCamera.hpp"
#include "Engine/Vector3.hpp"
#include "Engine/ParticleGenerator.hpp"
#include "Engine/Physics/Particle.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"
#include "Engine/Physics/RenderParticle.hpp"
#include "Engine/Physics/ForceGenerators/DragForceGenerator.hpp"
#include "Engine/RenderLine.hpp"
#include "Engine/Physics/Springs/AnchoredSpring.hpp"
#include "Engine/Physics/Springs/AnchoredBungee.hpp"
#include "Engine/Physics/Links/Chain.hpp"
#include "Engine/Physics/Links/AnchoredChain.hpp"

using namespace Physics;
using namespace Engine;

int main(void){

    /* Setup Variables */
    const float 
        particle_mass = 50.f,
        particle_start_z = 0.f;
    const int total_particles = 5;

    /* Asking User Input */
    float cable_length, particle_gap, particle_rad, grav_strength, force_x, force_y, force_z;
    std::cout << "Cable Length: "; std::cin >> cable_length;
    std::cout << "Particle Gap: "; std::cin >> particle_gap;
    std::cout << "Particle Radius: "; std::cin >> particle_rad;
    std::cout << "Gravity Strength: "; std::cin >> grav_strength;
    std::cout << "Apply Force" << std::endl;
    std::cout << "x: "; std::cin >> force_x;
    std::cout << "y: "; std::cin >> force_y;
    std::cout << "z: "; std::cin >> force_z;

    /* Initialize the Engine */
    Window* window = new Window("DynaBox", 800, 800);
    PhysicsWorld* world = new PhysicsWorld(Vector3(0.f, grav_strength, 0.f));
    GameEngine* engine = new GameEngine(
        window, world, new Shader("Shader/sample.vert", "Shader/sample.frag"));

    /* Scene Setup */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 400.f));
    orthoCam->setProjection(glm::ortho( - 400.f, 400.f, -400.f, 400.f, 0.1f, 1200.f)); 
    orthoCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    engine->Register(orthoCam, "orthoCam");
    engine->setActiveCamera("orthoCam");
    
    PerspectiveCamera* perspectiveCam = new PerspectiveCamera();
    perspectiveCam->setPosition(glm::vec3(0.f, -100.f, 400.f));
    perspectiveCam->setProjection(glm::perspective(glm::radians(120.f), 1.f, 0.1f, 1200.f));
    perspectiveCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    engine->Register(perspectiveCam, "perspectiveCam");

    Model* particleModel = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));

    float startPos = -(((total_particles - 1) / 2.f) * particle_gap);
    for (int i = 0; i < total_particles; i++) {

        RenderParticle* particle = new RenderParticle(new Particle(particle_mass, true), particleModel, Vector3(1.f, 0.f, 0.f));
        particle->SetRadius(particle_rad);
        particle->Base()->position = Vector3(startPos + particle_gap * i, 0.f, particle_start_z);

        if (i == 0) {
            Vector3* startForce = new Vector3(force_x, force_y, force_z);
            engine->passInfo(particle->Base(), startForce);
        }

        AnchoredChain* chain = new AnchoredChain();
        chain->length = cable_length;
        chain->anchorPoint = new Vector3(startPos + particle_gap * i, cable_length / 2.f, particle_start_z);
        chain->particles[0] = particle->Base();

        RenderLine* line = new RenderLine(
            chain->anchorPoint, 
            &(particle->Base()->position), 
            Vector3(1.f, 1.f, 1.f));

        engine->Register(particle);
        engine->Register(chain);
        engine->Register(line);
    }

    /* Run the Engine */
    engine->Run();

    return 0;
}