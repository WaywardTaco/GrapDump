
#include "RenderParticle.hpp"

using namespace Physics;

RenderParticle::RenderParticle(Particle* particle, Model* model) :
    particle(particle), model(model), customColor(NULL){}

RenderParticle::RenderParticle(Particle* particle, Model* model, Vector3 customColor) :
    particle(particle), model(model), customColor(new Vector3(customColor)){}

void RenderParticle::Update(double deltaTime){
    this->particle->Update(deltaTime);
}

void RenderParticle::Render(Shader* shader){
    if(!this->particle->isDestroyed()){
        this->model->setPosition((glm::vec3) this->particle->position);

        if(this->customColor != NULL)
            this->model->setBaseColor((glm::vec3) *(this->customColor));

        this->model->render(shader);
    }
}