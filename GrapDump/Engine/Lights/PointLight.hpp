
#pragma once

#include "LightSource.hpp"

namespace Engine {

class PointLight : public LightSource {
protected:
	glm::vec3 position;

public:
	PointLight();
	PointLight(glm::vec3 position);

	virtual void apply(Shader* shader);

	void rotateAround(glm::vec3 center, float degrees, glm::vec3 axis);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();

	void move(glm::vec3 move);
};
}