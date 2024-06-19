
#pragma once

#include "Camera.hpp"

class OrthoCamera : public Camera {

public:
	OrthoCamera();

	void setBounds(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar);
};