
#include "GameEngine.hpp"

using namespace Engine;

void GameEngine::Run(){
    constexpr std::chrono::nanoseconds timestep(16ms);

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    auto start_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    while(this->render_window->IsOpen()){
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time) ;
        prev_time = curr_time;

        curr_ns += dur;

        glfwPollEvents();
        if(curr_ns >= timestep){
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;
            
            this->Update(ms.count() / 1000.f);
        }
        this->physics_engine->ResetForces();

        this->Render();
    }

    glfwTerminate();
}

void GameEngine::RegisterShader(std::string shader_name, Shader* shader){
    this->registered_shaders[shader_name] = shader;
}

void GameEngine::RegisterParticle(Particle* particle){
    this->physics_engine->AddParticle(particle);
}

void GameEngine::RegisterParticle(RenderParticle* particle){
    this->render_particles.push_back(particle);
    this->RegisterParticle(particle->Base());
}

void GameEngine::RegisterParticle(LifespanParticle* particle){
    this->render_particles.push_back(particle);
    this->RegisterParticle(particle->Base());
}

void GameEngine::RemoveParticle(RenderParticle* particle){
    this->render_particles.remove(particle);
}

void GameEngine::SetActiveCam(Camera* camera){
    this->main_camera = camera;
}

void GameEngine::Update(double deltaTime){
    this->UpdateObjects(deltaTime);

    if(this->isPhysicsPaused)
        return;

    if (particle_generator != NULL) {
        int limit = particle_generator->toLimit(this->physics_engine->particles.size());
        for (int i = 0; i < limit; i++) this->RegisterParticle(this->particle_generator->GenerateLifespanParticle());
    }

    this->physics_engine->Update(deltaTime);
    
}

void GameEngine::UpdateObjects(double deltaTime){
    float camRotationAngle = 5.f;

    if(InputSystem::Instance()->key_Space){
        this->isPhysicsPaused = !this->isPhysicsPaused;
        InputSystem::Instance()->key_Space = false;
    }

    if(InputSystem::Instance()->key_1)
        this->main_camera = this->ortho_camera;
    if(InputSystem::Instance()->key_2)
        this->main_camera = this->perspective_camera;

    if(InputSystem::Instance()->key_W){
        this->main_camera->rotateAroundLocal((glm::vec3)Vector3(0.f,0.f,0.f), camRotationAngle, (glm::vec3)Vector3(1.f,0.f,0.f));
    }
    if(InputSystem::Instance()->key_S){
        this->main_camera->rotateAroundLocal((glm::vec3)Vector3(0.f,0.f,0.f), -camRotationAngle, (glm::vec3)Vector3(1.f,0.f,0.f));
    }

    if(InputSystem::Instance()->key_A){
        this->main_camera->rotateAround((glm::vec3)Vector3(0.f,0.f,0.f), -camRotationAngle, (glm::vec3)Vector3(0.f,1.f,0.f));
    }
    if(InputSystem::Instance()->key_D){
        this->main_camera->rotateAround((glm::vec3)Vector3(0.f,0.f,0.f), camRotationAngle, (glm::vec3)Vector3(0.f,1.f,0.f));
    }

}

void GameEngine::Render(){
    this->main_camera->apply(this->getShader("_mainShader"), NULL);

    for(RenderParticle* particle : this->render_particles) 
        particle->Render(this->getShader("_mainShader"));

    this->render_window->Update();
}

Shader* GameEngine::getShader(std::string shaderName){
    if(this->registered_shaders.count(shaderName) <= 0) return NULL;

    return this->registered_shaders[shaderName];
}

GameEngine::GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* orthoCam, Camera* perspectiveCam, Shader* mainShader, ParticleGenerator* particleGenerator) :
    isPhysicsPaused(false), render_window(renderWindow), physics_engine(physicsEngine), main_camera(orthoCam), ortho_camera(orthoCam), perspective_camera(perspectiveCam), particle_generator(particleGenerator)
{
    this->RegisterShader("_mainShader", mainShader);
    glfwSetKeyCallback(this->render_window->Base(), Engine::keyCallback);
}

GameEngine::~GameEngine(){
    for(std::unordered_map<std::string, Shader*>::iterator::value_type itr : this->registered_shaders)
        delete itr.second;
    this->registered_shaders.clear();

    for(RenderParticle* particle : this->render_particles)
        delete particle;
    this->render_particles.clear();

    delete this->render_window;
    delete this->physics_engine;
    delete this->main_camera;
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    bool isPressed = false;
    if(action == GLFW_PRESS){
        isPressed = true;
    }
    else if(action == GLFW_RELEASE)
        isPressed = false;

    switch(key){
        case GLFW_KEY_1:
            InputSystem::Instance()->key_1 = isPressed;
            break;
        case GLFW_KEY_2:
            InputSystem::Instance()->key_2 = isPressed;
            break;
        case GLFW_KEY_W:
            InputSystem::Instance()->key_W = isPressed;
            break;
        case GLFW_KEY_A:
            InputSystem::Instance()->key_A = isPressed;
            break;
        case GLFW_KEY_S:
            InputSystem::Instance()->key_S = isPressed;
            break;
        case GLFW_KEY_D:
            InputSystem::Instance()->key_D = isPressed;
            break;
        case GLFW_KEY_SPACE:
            InputSystem::Instance()->key_Space = isPressed;
            break;
    }   
}