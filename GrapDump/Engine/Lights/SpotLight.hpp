
#pragma once

#include "PointLight.hpp"

namespace Engine {

class SpotLight : public PointLight {
private: 
	glm::vec3 direction;
	float
		innerCutoff,
		outerCutoff;

public:
	SpotLight();

	void apply(Shader* shader);

	void setCutoffs(float inner, float outer);
	void setDirection(glm::vec3 direction);
};
}