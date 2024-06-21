
#include "PerspectiveCamera.hpp"

using namespace Engine;

PerspectiveCamera::PerspectiveCamera() :
	Camera(glm::perspective(glm::radians(100.f), 1.f, 0.1f, 100.f)),
	FOV(100.f), aspectRatio(1.f), zNear(0.1f), zFar(100.f) {};

void PerspectiveCamera::apply(Shader* object_shader, Shader* skybox_shader) {
	Camera::apply(object_shader, skybox_shader);
	if(skybox_shader != NULL)
		skybox_shader->passData("projection", this->_projection);
}

void PerspectiveCamera::setFOV(float FOV) {
	this->FOV = FOV;
	this->_projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setAspectRatio(float heightByWidth) {
	this->aspectRatio = heightByWidth;
	this->_projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setZnear(float zNear) {
	this->zNear = zNear;
	this->_projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}

void PerspectiveCamera::setZfar(float zFar) {
	this->zFar = zFar;
	this->_projection = glm::perspective(glm::radians(this->FOV), this->aspectRatio, this->zNear, this->zFar);
}