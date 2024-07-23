
#include "GameEngine.hpp"

using namespace Engine;

/* Constructor with object registration */
GameEngine::GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Shader* mainShader) :
    isPhysicsPaused(false), 
    render_window(renderWindow), 
    physics_engine(physicsEngine),
    active_camera(nullptr)
{
    /* Initializetion of shader, key callback, and randomizer */
    this->Register(mainShader, "_mainShader");
    glfwSetKeyCallback(this->render_window->Base(), Engine::keyCallback);
    srand((unsigned)time(NULL));
}

/* Deconstructor with proper cleaning */
GameEngine::~GameEngine(){
    for(std::unordered_map<std::string, Shader*>::iterator::value_type itr : this->registered_shaders)
        delete itr.second;
    this->registered_shaders.clear();

    for(std::unordered_map<std::string, Camera*>::iterator::value_type itr : this->registered_cameras)
        delete itr.second;
    this->registered_cameras.clear();

    for(std::unordered_map<std::string, GameObject*>::iterator::value_type itr : this->registered_objects)
        delete itr.second;
    this->registered_objects.clear();

    for(RenderParticle* renderparticle : this->render_particles)
        delete renderparticle;
    this->render_particles.clear();

    for(RenderLine* renderline : this->render_lines)
        delete renderline;
    this->render_lines.clear();

    delete this->render_window;
    delete this->physics_engine;
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

            /* Clearing of Forces after updates if unpaused */
            this->physics_engine->ResetForces();
        }

        /* Rendering */
        this->Render();
    }

    /* Termination of glfw post gameloop */
    glfwTerminate();
}

void GameEngine::setActiveCamera(std::string camera_name){
    if(this->registered_cameras.count(camera_name) > 0)
    this->active_camera = this->registered_cameras[camera_name];
}

void GameEngine::Register(Shader* shader, std::string name){
    /* Shaders are stored in a map based on their names */
    this->registered_shaders[name] = shader;
}

void GameEngine::Register(Camera* camera, std::string name){
    /* Cameras are stored in a map based on their names */
    this->registered_cameras[name] = camera;
}

void GameEngine::Register(GameObject* object, std::string name){
    /* Game Objects are stored in a map based on their names */
    this->registered_objects[name] = object;
}

void GameEngine::Register(RenderParticle* particle){
    /* Adds a particle to be rendered and simulated */
    this->render_particles.push_back(particle);
    this->physics_engine->AddParticle(particle->Base());
}

void GameEngine::Register(ForceGenerator* forceGenerator, RenderParticle* particle){
    /* Registers the connection between the force generator and the particle */
    this->physics_engine->forceRegistry.Add(particle->Base(), forceGenerator);
}

void GameEngine::Register(ParticleLink* link){
    /* Adds the link to the physics engine */
    this->physics_engine->links.push_back(link);
}

void GameEngine::Register(RenderLine* renderLine){
    /* RenderLines are in a list to be rendered */
    this->render_lines.push_back(renderLine);
}

void GameEngine::Update(double deltaTime){
    /* Updates all registered objects */
    this->UpdateObjects(deltaTime);

    /* Skips later physics computations */
    if(!this->isPhysicsPaused)
        /* Physics updates */
        this->physics_engine->Update(deltaTime);
}

void GameEngine::UpdateObjects(double deltaTime){
    float camRotationAngle = 5.f;

    /* Updates physics pausing */
    if(InputSystem::Instance()->key_Space && !this->wasStarted){
        this->wasStarted = true;

        firstParticle->AddForce(*startForce);
    }

    /* Camera switching */
    if(InputSystem::Instance()->key_1)
        this->setActiveCamera("orthoCam");
    if(InputSystem::Instance()->key_2)
        this->setActiveCamera("perspectiveCam");

    /* Camera movement */
    if(InputSystem::Instance()->key_W){
        this->active_camera->rotateAroundLocal((glm::vec3)Vector3(0.f,0.f,0.f), camRotationAngle, (glm::vec3)Vector3(1.f,0.f,0.f));
    }
    if(InputSystem::Instance()->key_S){
        this->active_camera->rotateAroundLocal((glm::vec3)Vector3(0.f,0.f,0.f), -camRotationAngle, (glm::vec3)Vector3(1.f,0.f,0.f));
    }
    if(InputSystem::Instance()->key_A){
        this->active_camera->rotateAround((glm::vec3)Vector3(0.f,0.f,0.f), -camRotationAngle, (glm::vec3)Vector3(0.f,1.f,0.f));
    }
    if(InputSystem::Instance()->key_D){
        this->active_camera->rotateAround((glm::vec3)Vector3(0.f,0.f,0.f), camRotationAngle, (glm::vec3)Vector3(0.f,1.f,0.f));
    }

}

void GameEngine::Render(){
    /* Applying the current camera to the shader */
    if(this->active_camera == nullptr) return;

    this->active_camera->apply(this->getShader("_mainShader"), NULL);

    /* Rendering all registered particles to the main window */
    for(RenderParticle* particle : this->render_particles) 
        particle->Render(this->getShader("_mainShader"));

    bool isUsingOrtho = this->active_camera == this->getCamera("orthoCam");
    for(RenderLine* line : this->render_lines)
        line->Render(this->getShader("_mainShader"), this->active_camera, isUsingOrtho);

    /* Updating to the next frame of the window */
    this->render_window->Update();
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

Camera* GameEngine::getCamera(std::string cameraName){
    /* Returns a named camera */
    if(this->registered_cameras.count(cameraName) <= 0) return NULL;
    return this->registered_cameras[cameraName];
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

/* For simulation start on space press */
void GameEngine::passInfo(Particle* firstParticle, Vector3* startForce) {
    this->firstParticle = firstParticle;
    this->startForce = startForce;
}