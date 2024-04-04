
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

class LightSource {
protected:
	glm::vec3
		color,
		ambientColor;
	float 
		brightness,
		ambientStrength,
		specularStrength,
		specularPhong;

public:
	LightSource();
	virtual void apply(Shader* shader) = 0;

	void setColor(glm::vec3 color);
	void setAmbientColor(glm::vec3 color);
	void setBrightness(float brightness);
	void adjustBrightness(float brightness);
	void setSpecStrength(float strength);
	void setSpecPhong(float phong);

};