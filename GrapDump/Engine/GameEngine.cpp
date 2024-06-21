
#include "GameEngine.hpp"

using namespace Engine;

/* Constructor with object registration */
GameEngine::GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* orthoCam, Camera* perspectiveCam, Shader* mainShader, ParticleGenerator* particleGenerator) :
    isPhysicsPaused(false), 
    render_window(renderWindow), 
    physics_engine(physicsEngine), 
    main_camera(orthoCam), 
    ortho_camera(orthoCam), 
    perspective_camera(perspectiveCam), 
    particle_generator(particleGenerator)
{
    /* Initializetion of shader, key callback, and randomizer */
    this->RegisterShader("_mainShader", mainShader);
    glfwSetKeyCallback(this->render_window->Base(), Engine::keyCallback);
    srand((unsigned)time(NULL));
}

/* Deconstructor with proper cleaning */
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

void GameEngine::Run(){
    /* Timestep Declaration */
    constexpr std::chrono::nanoseconds timestep(16ms);

    /* Initializing the clock */
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    auto start_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    /* Main simulator loop */
    while(this->render_window->IsOpen()){

        /* Delta time computation between frames */
        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time) ;
        prev_time = curr_time;

        curr_ns += dur;

        /* Input polling */
        glfwPollEvents();

        /* Update loop for timestep */
        if(curr_ns >= timestep){
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;
            
            this->Update(ms.count() / 1000.f);
        }
        /* Clearing of Forces after updates */
        this->physics_engine->ResetForces();

        /* Rendering */
        this->Render();
    }

    /* Termination of glfw post gameloop */
    glfwTerminate();
}

void GameEngine::Update(double deltaTime){

    /* Creates new particles via particle generator if able */
    if (particle_generator != NULL) {
        int limit = particle_generator->toLimit(this->physics_engine->particles.size());
        for (int i = 0; i < limit; i++) 
            this->RegisterParticle(this->particle_generator->GenerateLifespanParticle());
    }

    /* Skips later physics computations */
    if(!this->isPhysicsPaused)
        /* Physics updates */
        this->physics_engine->Update(deltaTime);

    /* Updates all registered objects */
    this->UpdateObjects(deltaTime);
    
}

void GameEngine::UpdateObjects(double deltaTime){
    float camRotationAngle = 5.f;

    /* Updates physics pausing */
    if(InputSystem::Instance()->key_Space){
        this->isPhysicsPaused = !this->isPhysicsPaused;
        InputSystem::Instance()->key_Space = false;
    }

    /* Camera switching */
    if(InputSystem::Instance()->key_1)
        this->main_camera = this->ortho_camera;
    if(InputSystem::Instance()->key_2)
        this->main_camera = this->perspective_camera;

    /* Camera movement */
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
    /* Applying the current camera to the shader */
    this->main_camera->apply(this->getShader("_mainShader"), NULL);

    /* Rendering all registered particles to the main window */
    for(RenderParticle* particle : this->render_particles) 
        particle->Render(this->getShader("_mainShader"));

    /* Updating to the next frame of the window */
    this->render_window->Update();
}

void GameEngine::RegisterShader(std::string shader_name, Shader* shader){
    /* Shaders are stored in a map based on their names */
    this->registered_shaders[shader_name] = shader;
}

void GameEngine::RegisterParticle(Particle* particle){
    /* Adds a particle to be simulated in physics engine */
    this->physics_engine->AddParticle(particle);
}

void GameEngine::RegisterParticle(RenderParticle* particle){
    /* Adds a particle to be rendered and simulated */
    this->render_particles.push_back(particle);
    this->RegisterParticle(particle->Base());
}

void GameEngine::RegisterParticle(LifespanParticle* particle){
    /* Adds a particle to be rendered and simulated */
    this->render_particles.push_back(particle);
    this->RegisterParticle(particle->Base());
}

void GameEngine::RemoveParticle(RenderParticle* particle){
    /* Removes a particle to be rendered */
    this->render_particles.remove(particle);
}

Shader* GameEngine::getShader(std::string shaderName){
    /* Returns a named shader */
    if(this->registered_shaders.count(shaderName) <= 0) return NULL;
    return this->registered_shaders[shaderName];
}

void Engine::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    /* Handles button inputs */
    bool isPressed = false;
    if(action == GLFW_PRESS){
        isPressed = true;
    }
    else if(action == GLFW_RELEASE)
        isPressed = false;

    /* Updates the InputSystem's members for checking elsewhere */
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