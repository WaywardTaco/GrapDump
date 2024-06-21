
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

using namespace Physics;
using namespace Engine;

int main(void){

    /* Scene Setup */
    Window* window = new Window("MP Phase 1 Group 3 - Josiah Aviso & Dun Baniqued", 800, 800);
    PhysicsWorld* world = new PhysicsWorld();

    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 400.f));
    orthoCam->setProjection(glm::ortho( - 400.f, 400.f, -400.f, 400.f, 0.1f, 800.f)); 
    orthoCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    
    PerspectiveCamera* perspectiveCam = new PerspectiveCamera();
    perspectiveCam->setPosition(glm::vec3(0.f, -100.f, 400.f));
    perspectiveCam->setProjection(glm::perspective(glm::radians(120.f), 1.f, 0.1f, 800.f));
    perspectiveCam->setCenter(glm::vec3(0.f, 0.f, 0.f));
    
    /* User Input for particle limit */
    int num;
    std::cout << "Enter Limit: "; std::cin >> num;
    ParticleGenerator* generator = new ParticleGenerator(num, Vector3(0.f, -300.f, 0.f));

    /* Initialize the Engine */
    GameEngine* engine = new GameEngine(
        window,
        world,
        orthoCam,
        perspectiveCam,
        new Shader("Shader/sample.vert", "Shader/sample.frag"),
        generator);

    /* Run the Engine */
    engine->Run();

    return 0;
}