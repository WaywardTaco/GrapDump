
#pragma once

#include "Camera.hpp"

class PerspectiveCamera : public Camera {
private:
	float
		FOV, aspectRatio, zNear, zFar;

public:
	PerspectiveCamera();

	void setFOV(float FOV);
	void setAspectRatio(float heightByWidth);
	void setZnear(float zNear);
	void setZfar(float zFar);
};