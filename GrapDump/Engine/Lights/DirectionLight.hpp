
#pragma once

#include "LightSource.hpp"

class DirectionLight : public LightSource {
private:
	glm::vec3 direction;

public:
	DirectionLight();
	DirectionLight(glm::vec3 direction);

	void apply(Shader* shader);
	void setDirection(glm::vec3 direction);
	void rotate(float degrees, glm::vec3 axis);
};