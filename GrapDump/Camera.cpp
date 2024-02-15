
#include "Camera.hpp"

Camera::Camera(float aspectRatio) : position(0.f, 0.f, 2.f), center(0.f) {
	this->projection = glm::perspective(glm::radians(100.f), aspectRatio, 0.1f, 100.f);
};

void Camera::move(glm::vec3 vector) {
	this->position.x += vector.x;
	this->position.y += vector.y;
	this->position.z += vector.z;
};

void Camera::pan(glm::vec3 vector) {
	this->center.x += vector.x;
	this->center.y += vector.y;
	this->center.z += vector.z;
};

void Camera::setProjection(glm::mat4 projection) {
	this->projection = projection;
};

glm::mat4 Camera::getView() {
	return glm::lookAt(this->position, this->center, glm::vec3(0.f, 1.f, 0.f));
};

glm::mat4 Camera::getProjection() {
	return this->projection;
};