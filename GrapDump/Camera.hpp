#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 center;
	glm::mat4 projection;

public:
	Camera(float aspectRatio);

public:
	void move(glm::vec3 vector);
	void pan(glm::vec3 vector);

public:
	void setProjection(glm::mat4 projection);

	glm::mat4 getView();
	glm::mat4 getProjection();
};