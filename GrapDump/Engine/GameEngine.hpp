
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
#include "GameObject.hpp"
#include "RenderLine.hpp"

namespace Engine {
    using namespace std::chrono_literals;
    using namespace Physics;
    class GameEngine {
        private:
            /* Tracker for physics pausing */
            bool isPhysicsPaused;

            /* Key Systems / Objects */
            Window* render_window;
            PhysicsWorld* physics_engine;
            Camera* active_camera;

            /* Objects to registered in the engine */
            std::unordered_map<std::string, Shader*> registered_shaders;
            std::unordered_map<std::string, Camera*> registered_cameras;
            std::unordered_map<std::string, GameObject*> registered_objects;

            std::list<RenderLine*> render_lines;
            std::list<RenderParticle*> render_particles;

        public:
            /* Constructor requiring relevant objects */
            GameEngine(Window* renderWindow, PhysicsWorld* physicsEngine, Shader* mainShader);
            ~GameEngine();

            void Run();

            void setActiveCamera(std::string camera_name);

            void Register(Shader* shader, std::string name);
            void Register(Camera* camera, std::string name);
            void Register(GameObject* object, std::string name);
            void Register(RenderParticle* particle);
            void Register(ForceGenerator* forceGenerator, RenderParticle* particle);
            void Register(ParticleLink* link);
            void Register(RenderLine* renderLine);

            void RemoveParticle(RenderParticle* particle);

        private:
            /* Gameloop utility functions */
            void Update(double deltaTime);
            void Render();
            void UpdateObjects(double deltaTime);

            /* Utility functions for internal logic */
            Shader* getShader(std::string shaderName);
            Camera* getCamera(std::string cameraName);
    };

    /* Callback function for key input */
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
}