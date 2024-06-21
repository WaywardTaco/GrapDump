
#include "SpotLight.hpp"
#include <iostream>

using namespace Engine;

SpotLight::SpotLight() :
	PointLight(), 
    direction({0.f, 0.f, 1.f}), 
    innerCutoff(15.f), 
    outerCutoff(30.f) {};

void SpotLight::apply(Shader* shader) {

    shader->passData("spotLight.direction", this->direction);
    shader->passData("spotLight.innerCutoff", this->innerCutoff);
    shader->passData("spotLight.outerCutoff", this->outerCutoff);

    shader->passData("spotLight.base.position", this->position);

    shader->passData("spotLight.base.color", this->color);
    shader->passData("spotLight.base.brightness", this->brightness);
    shader->passData("spotLight.base.ambientStr", this->ambientStrength);
    shader->passData("spotLight.base.ambientCol", this->ambientColor);
    shader->passData("spotLight.base.specStr", this->specularStrength);
    shader->passData("spotLight.base.specPhong", this->specularPhong);
}

void SpotLight::setCutoffs(float inner, float outer) {
	this->innerCutoff = inner;
	this->outerCutoff = outer;
}

void SpotLight::setDirection(glm::vec3 direction) {
    this->direction = glm::normalize(direction);
}