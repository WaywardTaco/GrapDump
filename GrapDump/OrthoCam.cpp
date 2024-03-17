
#include "OrthoCam.hpp"

OrthoCam::OrthoCam() :
	Camera(glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1.f)),
	left(-2.f), right(2.f), bottom(-2.f), top(2.f), zNear(-1.f), zFar(1.f) {}

void OrthoCam::setBounds(
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

void OrthoCam::setLeft(float left){
	this->left = left;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCam::setRight(float right) {
	this->right = right;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCam::setBottom(float bottom) {
	this->bottom = bottom;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCam::setTop(float top) {
	this->top = top;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCam::setZnear(float zNear) {
	this->zNear = zNear;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}

void OrthoCam::setZfar(float zFar) {
	this->zFar = zFar;
	this->projection = glm::ortho(this->left, this->right, this->bottom, this->top, this->zNear, this->zFar);
}