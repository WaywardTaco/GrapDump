
#include "Camera.hpp"

Camera::Camera(glm::mat4 projection) : 
    // Default Camera values
    position(0.f, 0.f, -1.f), worldUp(0.f, 1.f, 0.f) 
{
    this->setCenter({0.f, 0.f, 0.f});
	this->projection = projection;
};

void Camera::apply(GLuint shaderProgram) {
    // Application of camera values to shader program variables
    glUseProgram(shaderProgram);
    glm::mat4 viewMat = glm::lookAt(this->position, this->center, this->worldUp);

    unsigned int viewAdrs = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewAdrs, 1, GL_FALSE, glm::value_ptr(viewMat));

    unsigned int projectionAdrs = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionAdrs, 1, GL_FALSE, glm::value_ptr(this->projection));

    GLuint positionAdrs = glGetUniformLocation(shaderProgram, "cameraPos");
    glUniform3fv(positionAdrs, 1, glm::value_ptr(this->position));
};

void Camera::moveBy(glm::vec3 move) {
    this->position += move;
    this->center += move;
}

void Camera::moveTo(glm::vec3 move) {
    glm::vec3 displacement = move - this->position;

    this->position += displacement;
    this->center += displacement;
}

void Camera::rotateAround(float degrees, glm::vec3 axis) {
    // Creation of a matrix to transform axis of rotation with respect to the camera itself
    glm::vec3 forwardVec = glm::normalize(glm::vec3(this->position - this->center));
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, this->worldUp));
    glm::vec3 upVec = glm::normalize(glm::cross(rightVec, forwardVec));
    glm::mat4 cameraTransform = glm::mat4(1.f);

    cameraTransform[0][0] = rightVec.x;
    cameraTransform[0][1] = rightVec.y;
    cameraTransform[0][2] = rightVec.z;
    cameraTransform[1][0] = upVec.x;
    cameraTransform[1][1] = upVec.y;
    cameraTransform[1][2] = upVec.z;
    cameraTransform[2][0] = forwardVec.x;
    cameraTransform[2][1] = forwardVec.y;
    cameraTransform[2][2] = forwardVec.z;

    // Input Axis transformed with respect to camera
    glm::vec3 transformedAxis = glm::normalize(glm::vec3(cameraTransform * glm::vec4(glm::normalize(axis), 1.f)));

    // Current position rotated about with respect to the camera's orientation and position
    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), transformedAxis);
    this->position = glm::vec3(positionTransform * glm::vec4(this->position, 1.f));

    // World up updated for consistency across more orientations
    this->worldUp = glm::vec3(positionTransform * glm::vec4(this->worldUp, 1.f));
}

void Camera::setPosition(glm::vec3 position) {
    this->position = position;
};

void Camera::setCenter(glm::vec3 center) {
    this->center = center;
};

void Camera::setProjection(glm::mat4 projection) {
    this->projection = projection;
};

void Camera::setWorldUp(glm::vec3 worldUp) {
    this->worldUp = glm::normalize(worldUp);
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(this->position, this->center, this->worldUp);
}

glm::mat4 Camera::getProjection() {
    return this->projection;
}

glm::vec3 Camera::getPosition() {
    return this->position;
}