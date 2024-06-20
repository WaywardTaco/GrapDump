
#pragma once

#include <chrono>
#include <unordered_map>
#include <list>

#include "Window.hpp"
#include "Shader.hpp"
#include "Cameras/Camera.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Physics/RenderParticle.hpp"
#include "Physics/LifespanParticle.hpp"
#include "ParticleGenerator.hpp"

namespace Engine {
    using namespace std::chrono_literals;
    using namespace Physics;
    class GameEngine {
        private:
            Window* render_window;
            PhysicsWorld* physics_engine;
            Camera* main_camera;
            ParticleGenerator* particle_generator = NULL;
            std::unordered_map<std::string, Shader*> registered_shaders;
            std::list<RenderParticle*> render_particles;

        public:
            GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* mainCamera, Shader* mainShader);
            GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* mainCamera, Shader* mainShader, ParticleGenerator* particleGenerator);
            ~GameEngine();

            void Run();
            void RegisterShader(std::string shader_name, Shader* shader);
            void RegisterParticle(Particle* particle);
            void RegisterParticle(RenderParticle* particle);
            void RegisterParticle(LifespanParticle* particle);
            void RemoveParticle(RenderParticle* particle);

            void SetActiveCam(Camera* camera);

        private:
            void Update(double deltaTime);
            void Render();
            Shader* getShader(std::string shaderName);
    };
}