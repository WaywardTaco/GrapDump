
#include "DirectionLight.hpp"

DirectionLight::DirectionLight()
	: LightSource(), direction({ 0.f, -1.f, 0.f }) {}

DirectionLight::DirectionLight(glm::vec3 direction)
	: LightSource(), direction(glm::normalize(direction)) {}

void DirectionLight::apply(Shader* shader) {
	LightSource::apply(shader);

}

void DirectionLight::setDirection(glm::vec3 direction) {
	this->direction = glm::normalize(direction);
}

void DirectionLight::rotate(float degrees, glm::vec3 axis) {
	glm::mat4 transform = glm::rotate(glm::mat4(1.f), glm::radians(degrees), glm::normalize(axis));
	this->direction = glm::normalize(glm::vec3(transform * glm::vec4(this->direction, 1.f)));
}