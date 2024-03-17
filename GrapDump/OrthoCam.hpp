
#pragma once

#include "Camera.hpp"

class OrthoCam : public Camera {
private:
	float
		left, right, bottom, top, zNear, zFar;

public:
	OrthoCam();

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