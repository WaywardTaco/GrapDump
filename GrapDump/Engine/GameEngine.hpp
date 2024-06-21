
#pragma once

#include <chrono>
#include <unordered_map>
#include <list>

#include "Window.hpp"
#include "Shader.hpp"
#include "InputSystem.hpp"
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
            /* Tracker for physics pausing */
            bool isPhysicsPaused;

            /* Objects to be registered in the engine */
            Window* render_window;
            PhysicsWorld* physics_engine;
            Camera* main_camera;
            Camera* ortho_camera;
            Camera* perspective_camera;
            ParticleGenerator* particle_generator = NULL;
            std::unordered_map<std::string, Shader*> registered_shaders;
            std::list<RenderParticle*> render_particles;

        public:
            /* Constructor requiring relevant objects */
            GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Camera* orthoCam, Camera* perspectiveCam, Shader* mainShader, ParticleGenerator* particleGenerator);
            ~GameEngine();

            void Run();
            void RegisterParticle(LifespanParticle* particle);
            void RemoveParticle(RenderParticle* particle);

        private:
            /* Gameloop utility functions */
            void Update(double deltaTime);
            void Render();
            void UpdateObjects(double deltaTime);

            /* Utility functions for internal logic */
            void RegisterShader(std::string shader_name, Shader* shader);
            void RegisterParticle(Particle* particle);
            void RegisterParticle(RenderParticle* particle);

            Shader* getShader(std::string shaderName);
    };

    /* Callback function for key input */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}