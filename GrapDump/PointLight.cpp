
#include "PointLight.hpp"

PointLight::PointLight() 
	: LightSource(), position(0.f) {}

PointLight::PointLight(glm::vec3 position)
	: LightSource(), position(position) {}

void PointLight::apply(GLuint shaderProgram) {
    // Setting shader variables for the point light
    glUseProgram(shaderProgram);

	GLuint posAdrs = glGetUniformLocation(shaderProgram, "pointLight.position");
	glUniform3fv(posAdrs, 1, glm::value_ptr(this->position));

    GLuint colorAdrs = glGetUniformLocation(shaderProgram, "pointLight.color");
    glUniform3fv(colorAdrs, 1, glm::value_ptr(this->color));

    GLuint brightAdrs = glGetUniformLocation(shaderProgram, "pointLight.brightness");
    glUniform1f(brightAdrs, this->brightness);

    GLuint ambStrAdrs = glGetUniformLocation(shaderProgram, "pointLight.ambientStr");
    glUniform1f(ambStrAdrs, this->ambientStrength);

    GLuint ambColorAdrs = glGetUniformLocation(shaderProgram, "pointLight.ambientCol");
    glUniform3fv(ambColorAdrs, 1, glm::value_ptr(this->ambientColor));

    GLuint specStrAdrs = glGetUniformLocation(shaderProgram, "pointLight.specStr");
    glUniform1f(specStrAdrs, this->specularStrength);

    GLuint specPhongAdrs = glGetUniformLocation(shaderProgram, "pointLight.specPhong");
    glUniform1f(specPhongAdrs, this->specularPhong);
}

void PointLight::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    // Rotation of the light w/ respect to the origin
    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), glm::normalize(axis));
    this->position = glm::vec3(positionTransform * glm::vec4(this->position, 1.f));
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
}

void PointLight::move(glm::vec3 move) {
	this->position += move;
}