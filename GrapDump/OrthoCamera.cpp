
#include "OrthoCamera.hpp"

OrthoCamera::OrthoCamera() :
	// Initial Orthographic Values
	Camera(glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f)),
	left(-1.f), right(1.f), bottom(-1.f), top(1.f), zNear(0.1f), zFar(100.f) {}

void OrthoCamera::setBounds(
	float left,
	float right,
	float bottom,
	float top,
	float zNear,
	float zFar
) {
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->zNear = zNear;
	this->zFar = zFar;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setLeft(float left){
	this->left = left;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setRight(float right) {
	this->right = right;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setBottom(float bottom) {
	this->bottom = bottom;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setTop(float top) {
	this->top = top;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setZnear(float zNear) {
	this->zNear = zNear;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCamera::setZfar(float zFar) {
	this->zFar = zFar;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}