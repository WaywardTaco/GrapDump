
#include "PerspectiveCamera.hpp"

PerspectiveCamera::PerspectiveCamera() :
	Camera(glm::perspective(glm::radians(100.f), 1.f, 0.1f, 100.f)),
	FOV(100.f), aspectRatio(1.f), zNear(0.1f), zFar(100.f) {};

void PerspectiveCamera::apply(Shader* object_shader, Shader* skybox_shader) {
	Camera::apply(object_shader, skybox_shader);
	skybox_shader->passMat4("projection", this->projection);
}

void PerspectiveCamera::setFOV(float FOV) {
	this->FOV = FOV;
	this->projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setAspectRatio(float heightByWidth) {
	this->aspectRatio = heightByWidth;
	this->projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setZnear(float zNear) {
	this->zNear = zNear;
	this->projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setZfar(float zFar) {
	this->zFar = zFar;
	this->projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}