
#include "OrthoCamera.hpp"

OrthoCamera::OrthoCamera() :
	Camera(glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f)){}

void OrthoCamera::setBounds(
	float left,
	float right,
	float bottom,
	float top,
	float zNear,
	float zFar
) {
	this->projection = glm::ortho(left, right, bottom, top, zNear, zFar);
}