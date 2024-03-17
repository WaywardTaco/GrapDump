
#pragma once

#include "LightSource.hpp"

class PointLight : public LightSource {
private:
	glm::vec3 position;

public:
	PointLight();
	PointLight(glm::vec3 position);

	void apply(GLuint shaderProgram);

	void setPosition(glm::vec3 position);
	void move(glm::vec3 move);
};