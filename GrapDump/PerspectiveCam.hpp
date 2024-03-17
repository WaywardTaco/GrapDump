
#pragma once

#include "Camera.hpp"

class PerspectiveCam : public Camera {
private:
	float
		FOV, aspectRatio, zNear, zFar;

public:
	PerspectiveCam();

	void setFOV(float FOV);
	void setAspectRatio(float heightByWidth);
	void setZnear(float zNear);
	void setZfar(float zFar);
};