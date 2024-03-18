
#pragma once

#include "Camera.hpp"

class OrthoCamera : public Camera {
private:
	float
		left, right, bottom, top, zNear, zFar;

public:
	OrthoCamera();

	void setBounds(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar);
	void setLeft(float left);
	void setRight(float right);
	void setBottom(float bottom);
	void setTop(float top);
	void setZnear(float zNear);
	void setZfar(float zFar);
};