
#include "Camera.hpp"

using namespace Engine;

Camera::Camera(glm::mat4 projection)
: _position(0.f, 0.f, -1.f), _worldUp(0.f, 1.f, 0.f) {
    this->setCenter({0.f, 0.f, 0.f});
	this->_projection = projection;
};

void Camera::apply(Shader* object_shader, Shader* skybox_shader) {
    object_shader->use();
    glm::mat4 viewMat = glm::lookAt(this->_position, this->_center, this->_worldUp);

    object_shader->passData("view", viewMat);
    object_shader->passData("projection", this->_projection);
    object_shader->passData("cameraPos", this->_position);

    if (skybox_shader == NULL)
        return;

    skybox_shader->use();
    glm::mat4 skyMatrix = glm::mat4(1.f);
    skyMatrix = glm::mat4(glm::mat3(viewMat));

    skybox_shader->passData("view", skyMatrix);
};

void Camera::moveBy(glm::vec3 move) {
    this->_position += move;
    this->_center += move;
}

void Camera::moveTo(glm::vec3 move) {
    glm::vec3 displacement = move - this->_position;

    this->_position += displacement;
    this->_center += displacement;
}

void Camera::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), glm::normalize(axis));
    this->_position = center + glm::vec3(positionTransform * glm::vec4(this->_position - center, 1.f));
    this->_center = center + glm::vec3(positionTransform * glm::vec4(this->_center - center, 1.f));
}

void Camera::rotateAroundLocal(glm::vec3 center, float degrees, glm::vec3 axis) {
    // Creation of a matrix to transform axis of rotation with respect to the camera itself
    glm::vec3 forwardVec = glm::normalize(glm::vec3(this->_position - this->_center));
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, this->_worldUp));
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
    this->_position = glm::vec3(positionTransform * glm::vec4(this->_position, 1.f));
}

void Camera::turn(float degrees, glm::vec3 axis) {  
    glm::vec3 forwardVec = glm::normalize(glm::vec3(this->_center - this->_position));
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, this->_worldUp));

    glm::vec3 upVec = glm::normalize(glm::cross(rightVec, forwardVec));

    glm::mat4 cameraTransform = glm::mat4(1.f);

    // [Col][Row]
    cameraTransform[0][0] = rightVec.x;
    cameraTransform[0][1] = rightVec.y;
    cameraTransform[0][2] = rightVec.z;

    cameraTransform[1][0] = upVec.x;
    cameraTransform[1][1] = upVec.y;
    cameraTransform[1][2] = upVec.z;

    cameraTransform[2][0] = forwardVec.x;
    cameraTransform[2][1] = forwardVec.y;
    cameraTransform[2][2] = forwardVec.z;

    glm::vec4 centerTransformed = cameraTransform * glm::vec4(this->_center, 1.0);

    glm::mat4 centerTransform = glm::mat4(1.f);
    centerTransform = glm::rotate(centerTransform, glm::radians(degrees), glm::normalize(axis));

    this->_center = glm::vec3(glm::inverse(cameraTransform) * centerTransform * centerTransformed);

    this->_worldUp = glm::vec3(centerTransform * glm::vec4(this->_worldUp, 1.f));
}

void Camera::setPosition(glm::vec3 position) {
    this->_position = position;
};

void Camera::setCenter(glm::vec3 center) {
    this->_center = center;
};

void Camera::setProjection(glm::mat4 projection) {
    this->_projection = projection;
};

void Camera::setWorldUp(glm::vec3 worldUp) {
    this->_worldUp = worldUp;
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(this->_position, this->_center, this->_worldUp);
}

glm::vec3 Camera::getCenter() {
    return this->_center;
}
