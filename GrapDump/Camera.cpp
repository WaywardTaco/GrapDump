
#include "Camera.hpp"


glm::vec3 rotation;

Camera::Camera(glm::mat4 projection)
 : position(0.f, 0.f, -1.f), worldUp(0.f, 1.f, 0.f) {
    this->setCenter({0.f, 0.f, 0.f});
	this->projection = projection;
};

void Camera::apply(GLuint shaderProgram) {
    glm::mat4 cameraMat = glm::lookAt(this->position, this->center, this->worldUp);

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cameraMat));

    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));
};

void Camera::adjustCenterDist(double adjust) {
    this->setCenterDist(this->centerDist + adjust);
};

void Camera::absMove(glm::vec3 move) {
    this->setPosition(this->position + move);
};

void Camera::relMove(glm::vec3 move) {

};

void Camera::absRotate(glm::vec3 rotate) {
    glm::mat4 rotationMat(1.f);
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.x), { 1.f, 0.f, 0.f });
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.y), { 0.f, 1.f, 0.f });
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.z), { 0.f, 0.f, 1.f });

    this->setRotation(rotationMat * glm::vec4(this->getRotation(), 1.f));
};

void Camera::relRotate(glm::vec3 rotate) {
    glm::mat4 rotationMat(1.f);
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.x), { 1.f, 0.f, 0.f });
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.y), { 0.f, 1.f, 0.f });
    rotationMat = glm::rotate(rotationMat, glm::radians(rotate.z), { 0.f, 0.f, 1.f });

    this->setRotation(rotationMat * glm::vec4(this->getRotation(), 1.f));
};

void Camera::absMoveCenter(glm::vec3 move) {
    this->setCenter(this->center + move);
};

void Camera::relMoveCenter(glm::vec3 move) {

};

void Camera::setCenterDist(double centerDist) {
    
};

void Camera::setPosition(glm::vec3 position) {
    this->position = position;

    this->centerDist = glm::length(this->position - this->center);
};

void Camera::setRotation(glm::vec3 rotation) {

};

void Camera::setCenter(glm::vec3 center) {
    this->center = center;

    this->centerDist = glm::length(this->position - this->center);
};

void Camera::setWorldUp(glm::vec3 worldUp) {
    this->worldUp = worldUp;
};

void Camera::setProjection(glm::mat4 projection) {
    this->projection = projection;
};

glm::vec3 Camera::getRotation() {
    return glm::normalize(glm::vec3(this->center - this->position));
};