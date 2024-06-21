
#include "RenderParticle.hpp"

using namespace Physics;

RenderParticle::RenderParticle(Particle* particle, Model* model) :
    particle(particle), model(model), customColor(NULL){}

RenderParticle::RenderParticle(Particle* particle, Model* model, Vector3 customColor) :
    particle(particle), model(model), customColor(new Vector3(customColor)){}

RenderParticle::RenderParticle(Particle* particle, Model* model, Vector3 customColor, float scale) :
    particle(particle), model(model), customColor(new Vector3(customColor)), scale(scale) {}


void RenderParticle::Update(double deltaTime){
    this->particle->Update(deltaTime);
}

void RenderParticle::Render(Shader* shader){
    /* Renders the particle's base to the particle's position and based on color */
    if(!this->particle->isDestroyed()){
        this->model->setPosition((glm::vec3) this->particle->position);

        if(this->customColor != NULL)
            this->model->setBaseColor((glm::vec3) *(this->customColor));

        if (this->scale != NULL)
            this->model->setScale(this->scale);

        this->model->render(shader);
    }
}

Particle* RenderParticle::Base(){
    return this->particle;
}