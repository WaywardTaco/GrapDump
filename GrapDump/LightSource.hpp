
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class LightSource {
private:
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
	virtual void apply(GLuint shaderProgram);

	void setColor(glm::vec3 color);
	void setAmbientColor(glm::vec3 color);
	void setBrightness(float brightness);
	void setSpecStrength(float strength);
	void setSpecPhong(float phong);

};