
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
#include "Engine/Physics/Particle.hpp"
#include "Engine/Physics/PhysicsWorld.hpp"
#include "Engine/Physics/RenderParticle.hpp"
#include "Engine/Physics/ForceGenerators/DragForceGenerator.hpp"
//#include "Skybox.hpp"
//#include "LightSource.hpp"
//#include "PointLight.hpp"
//#include "DirectionLight.hpp"
//#include "SpotLight.hpp"
//#include "Player.hpp"

using namespace Physics;
using namespace Engine;

int main(void){
    /* Object Declarations */
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition(glm::vec3(0.f, 0.f, 350.f));
    orthoCam->setProjection(glm::ortho( - 350.f, 350.f, -350.f, 350.f, 0.1f, 700.f));    
    
    PerspectiveCamera* perspectiveCam = new PerspectiveCamera();
    perspectiveCam->setPosition(glm::vec3(0.f, 0.f, 350.f));
    perspectiveCam->setProjection(glm::perspective(glm::radians(120.f), 1.f, 0.1f, 700.f));
    
    Window* window = new Window("PC01 Josiah Aviso & Dun Baniqued", 700, 700);
    PhysicsWorld* world = new PhysicsWorld();

    GameEngine* engine = new GameEngine(
        window, 
        world, 
        orthoCam,
        new Shader("Shader/sample.vert", "Shader/sample.frag"));

    Model* particleSphere = new Model("3D/sphere.obj", glm::vec3(0.f, 0.f, 0.f));
    particleSphere->setScale(15.f);

    for(int i = 0; i < 10; i++){
        RenderParticle* particle = new RenderParticle(new Particle(1.0f), particleSphere, Vector3(1.0, 0.0 + i, 1.0 - i));
        particle->Base()->position = Vector3(0.0, 1.0 + i*50, 0.0);
        engine->RegisterParticle(particle);
    }

    engine->Run();

    return 0;
}