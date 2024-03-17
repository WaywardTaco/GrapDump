
#include "PointLight.hpp"

PointLight::PointLight() 
	: LightSource(), position(0.f) {}

PointLight::PointLight(glm::vec3 position)
	: LightSource(), position(position) {}

void PointLight::apply(GLuint shaderProgram) {
	LightSource::apply(shaderProgram);

	GLuint lightAdrs = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3fv(lightAdrs, 1, glm::value_ptr(this->position));
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
}

void PointLight::move(glm::vec3 move) {
	this->position += move;
}