
#include "Camera.hpp"

Camera::Camera(glm::mat4 projection)
 : position(0.f, 0.f, -1.f), worldUp(0.f, 1.f, 0.f) {
    this->setCenter({0.f, 0.f, 0.f});
	this->projection = projection;
};

void Camera::apply(Shader* object_shader, Shader* skybox_shader) {
    object_shader->use();
    glm::mat4 viewMat = glm::lookAt(this->position, this->center, this->worldUp);

    object_shader->passMat4("view", viewMat);
    object_shader->passMat4("projection", this->projection);
    object_shader->passVec3("cameraPos", this->position);

    if (skybox_shader == NULL)
        return;

    skybox_shader->use();
    glm::mat4 skyMatrix = glm::mat4(1.f);
    skyMatrix = glm::mat4(glm::mat3(viewMat));

    skybox_shader->passMat4("view", skyMatrix);
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

void Camera::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), glm::normalize(axis));
    this->position = center + glm::vec3(positionTransform * glm::vec4(this->position - center, 1.f));
    this->center = center + glm::vec3(positionTransform * glm::vec4(this->center - center, 1.f));
}

void Camera::turn(float degrees, glm::vec3 axis) {  
    glm::vec3 forwardVec = glm::normalize(glm::vec3(this->center - this->position));
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, this->worldUp));

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

    glm::vec4 centerTransformed = cameraTransform * glm::vec4(this->center, 1.0);

    glm::mat4 centerTransform = glm::mat4(1.f);
    centerTransform = glm::rotate(centerTransform, glm::radians(degrees), glm::normalize(axis));

    this->center = glm::vec3(glm::inverse(cameraTransform) * centerTransform * centerTransformed);

    this->worldUp = glm::vec3(centerTransform * glm::vec4(this->worldUp, 1.f));
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
    this->worldUp = worldUp;
}

glm::mat4 Camera::getViewMat() {
    return glm::lookAt(this->position, this->center, this->worldUp);
}

glm::vec3 Camera::getCenter() {
    return this->center;
}
