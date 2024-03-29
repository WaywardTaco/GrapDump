#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
protected:
	glm::vec3 
		position,
		center,
		worldUp;

	glm::mat4 projection;

public:
	Camera(glm::mat4 projection);

public:
	void apply(GLuint shaderProgram);

	void moveBy(glm::vec3 move);
	void moveTo(glm::vec3 move);
	void rotateAround(float degrees, glm::vec3 axis);

	void setPosition(glm::vec3 position);
	void setCenter(glm::vec3 center);
	void setProjection(glm::mat4 projection);
	void setWorldUp(glm::vec3 worldUp);

	glm::mat4 getViewMat();
	glm::mat4 getProjection();
	glm::vec3 getPosition();

};