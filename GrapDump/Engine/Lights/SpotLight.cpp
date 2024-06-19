
#include "SpotLight.hpp"
#include <iostream>

SpotLight::SpotLight() :
	PointLight(), 
    direction({0.f, 0.f, 1.f}), 
    innerCutoff(15.f), 
    outerCutoff(30.f) {};

void SpotLight::apply(Shader* shader) {

    shader->passVec3("spotLight.direction", this->direction);
    shader->passFloat("spotLight.innerCutoff", this->innerCutoff);
    shader->passFloat("spotLight.outerCutoff", this->outerCutoff);

    shader->passVec3("spotLight.base.position", this->position);

    shader->passVec3("spotLight.base.color", this->color);
    shader->passFloat("spotLight.base.brightness", this->brightness);
    shader->passFloat("spotLight.base.ambientStr", this->ambientStrength);
    shader->passVec3("spotLight.base.ambientCol", this->ambientColor);
    shader->passFloat("spotLight.base.specStr", this->specularStrength);
    shader->passFloat("spotLight.base.specPhong", this->specularPhong);
}

void SpotLight::setCutoffs(float inner, float outer) {
	this->innerCutoff = inner;
	this->outerCutoff = outer;
}

void SpotLight::setDirection(glm::vec3 direction) {
    this->direction = glm::normalize(direction);
}