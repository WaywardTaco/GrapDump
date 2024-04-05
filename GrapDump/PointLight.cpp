
#include "PointLight.hpp"

PointLight::PointLight() 
	: LightSource(), position(0.f) {}

PointLight::PointLight(glm::vec3 position)
	: LightSource(), position(position) {}

void PointLight::apply(Shader* shader) {
    shader->passVec3("pointLight.position", this->position);

    shader->passVec3("pointLight.color", this->color);
    shader->passFloat("pointLight.brightness", this->brightness);
    shader->passFloat("pointLight.ambientStr", this->ambientStrength);
    shader->passVec3("pointLight.ambientCol", this->ambientColor);
    shader->passFloat("pointLight.specStr", this->specularStrength);
    shader->passFloat("pointLight.specPhong", this->specularPhong);
}

void PointLight::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), glm::normalize(axis));
    this->position = center + glm::vec3(positionTransform * glm::vec4(this->position - center, 1.f));
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 PointLight::getPosition() {
    return this->position;
}

void PointLight::move(glm::vec3 move) {
	this->position += move;
}