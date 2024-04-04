
#include "PointLight.hpp"

PointLight::PointLight() 
	: LightSource(), position(0.f) {}

PointLight::PointLight(glm::vec3 position)
	: LightSource(), position(position) {}

void PointLight::apply(Shader* shader) {
	LightSource::apply(shader);
    shader->passVec3("lightPos", this->position);
}

void PointLight::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    glm::vec3 worldUp = { 0.f, 1.f, 0.f };

    glm::vec3 forwardVec = { 0.f, 0.f, 1.f };
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, worldUp));

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

    glm::vec3 transformedAxis = glm::normalize(glm::vec3(cameraTransform * glm::vec4(glm::normalize(axis), 1.f)));


    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), transformedAxis);

    this->position = glm::vec3(positionTransform * glm::vec4(this->position, 1.f));
    
    // Get the basis vectors centered on the camera center
    // Turn into a basis transform matrix
    // Find the axis of rotation in that new basis
    // Rotate the basis across that axis
    // Apply transform to the relative position of the camera


    // Transform the camera position with respect to the axis of transform of the center (ie. figure out the rotation about the axis in the new vector space)
    // Then apply the same transform as usual to the postion with respect to that axis to the camera
}

void PointLight::setPosition(glm::vec3 position) {
	this->position = position;
}

void PointLight::move(glm::vec3 move) {
	this->position += move;
}