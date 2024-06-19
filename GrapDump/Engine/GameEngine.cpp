
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

void GameEngine::RegisterRenderParticle(RenderParticle* particle){
    this->render_particles.push_back(particle);
    this->physics_engine->AddParticle(particle->Base());
}

void GameEngine::RemoveParticle(RenderParticle* particle){
    this->render_particles.remove(particle);
}

void GameEngine::Update(double deltaTime){
    this->physics_engine->Update(deltaTime);
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

GameEngine::GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* mainCamera, Shader* mainShader) :
    render_window(renderWindow), physics_engine(physicsEngine), main_camera(mainCamera)
{
    this->RegisterShader("_mainShader", mainShader);
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