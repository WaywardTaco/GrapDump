
#pragma once

#include "LightSource.hpp"

class PointLight : public LightSource {
private:
	glm::vec3 position;

public:
	PointLight();
	PointLight(glm::vec3 position);

	void apply(Shader* shader);

	void rotateAround(glm::vec3 center, float degrees, glm::vec3 axis);

	void setPosition(glm::vec3 position);
	void move(glm::vec3 move);
};