
#include "DirectionLight.hpp"

DirectionLight::DirectionLight()
    // Default Direction Light from straight above
	: LightSource(), direction({ 0.f, -1.f, 0.f }) {}

DirectionLight::DirectionLight(glm::vec3 direction) : LightSource(), 
    // Inputed Direction is normalized for consistency
    direction(glm::normalize(direction)) {}

void DirectionLight::apply(GLuint shaderProgram) {
    // Application of Direction Light Values to shader program variables
    glUseProgram(shaderProgram);

    GLuint dirAdrs = glGetUniformLocation(shaderProgram, "dirLight.direction");
    glUniform3fv(dirAdrs, 1, glm::value_ptr(this->direction));

    GLuint colorAdrs = glGetUniformLocation(shaderProgram, "dirLight.color");
    glUniform3fv(colorAdrs, 1, glm::value_ptr(this->color));

    GLuint brightAdrs = glGetUniformLocation(shaderProgram, "dirLight.brightness");
    glUniform1f(brightAdrs, this->brightness);

    GLuint ambStrAdrs = glGetUniformLocation(shaderProgram, "dirlight.ambientStr");
    glUniform1f(ambStrAdrs, this->ambientStrength);

    GLuint ambColorAdrs = glGetUniformLocation(shaderProgram, "dirLight.ambientCol");
    glUniform3fv(ambColorAdrs, 1, glm::value_ptr(this->ambientColor));

    GLuint specStrAdrs = glGetUniformLocation(shaderProgram, "dirLight.specStr");
    glUniform1f(specStrAdrs, this->specularStrength);

    GLuint specPhongAdrs = glGetUniformLocation(shaderProgram, "dirLight.specPhong");
    glUniform1f(specPhongAdrs, this->specularPhong);
}

void DirectionLight::setDirection(glm::vec3 direction) {
	this->direction = glm::normalize(direction);
}

void DirectionLight::rotate(float degrees, glm::vec3 axis) {
    // Rotation of direction w/ respect to world origin
	glm::mat4 transform = glm::rotate(glm::mat4(1.f), glm::radians(degrees), glm::normalize(axis));
	this->direction = glm::normalize(glm::vec3(transform * glm::vec4(this->direction, 1.f)));
}