
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

using namespace Physics;
using namespace Engine;

int main(void){

    /* Initialize the Engine */
    Window* window = new Window("MP Phase 1 Group 3 - Josiah Aviso & Dun Baniqued", 800, 800);
    PhysicsWorld* world = new PhysicsWorld();
    GameEngine* engine = new GameEngine(
        window, world, new Shader("Shader/sample.vert", "Shader/sample.frag"));

    /* Scene Setup */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 400.f));
    orthoCam->setProjection(glm::ortho( - 400.f, 400.f, -400.f, 400.f, 0.1f, 800.f)); 
    orthoCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    engine->Register(orthoCam, "orthoCam");
    engine->setActiveCamera("orthoCam");
    
    PerspectiveCamera* perspectiveCam = new PerspectiveCamera();
    perspectiveCam->setPosition(glm::vec3(0.f, -100.f, 400.f));
    perspectiveCam->setProjection(glm::perspective(glm::radians(120.f), 1.f, 0.1f, 800.f));
    perspectiveCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    
    /* User Input for particle limit */
    // int num;
    // std::cout << "Enter Particle Limit: "; std::cin >> num;
    // ParticleGenerator* generator = new ParticleGenerator(num, Vector3(0.f, -300.f, 0.f));

    Model* particleModel = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));
    RenderParticle* particle1 = new RenderParticle(new Particle(10.f, true), particleModel, Vector3(1.f, 0.f, 0.f));
    particle1->SetRadius(50.f);
    particle1->Base()->position = Vector3(-150.f, -150.f, 0.f);

    RenderParticle* particle2 = new RenderParticle(new Particle(10.f, false), particleModel, Vector3(0.f, 1.f, 0.f));
    particle2->SetRadius(50.f);
    particle2->Base()->position = Vector3(150.f, 150.f, 0.f);

    RenderParticle* particle3 = new RenderParticle(new Particle(10.f, true), particleModel, Vector3(0.f, 0.f, 1.f));
    particle3->SetRadius(50.f);
    particle3->Base()->position = Vector3(150.f, -150.f, 0.f);

    //world->AddParticle(particle1);
    //world->AttachToSpring(particle1);

    engine->Register(particle1);
    engine->Register(particle2);
    engine->Register(particle3);

    AnchoredSpring spring = AnchoredSpring(Vector3(-150.f, 150.f, 0.f), 0.3f, 100.f);
    engine->Register(&spring, particle1);

    Rod rod = Rod();
    rod.length = 300.f;
    rod.particles[0] = particle2->Base();
    rod.particles[1] = particle3->Base();
    engine->Register(&rod);


    /* Run the Engine */
    engine->Run();

    return 0;
}