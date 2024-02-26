#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 worldUp;

public:
	Camera();

	void setPosition(glm::vec3 position);
	void setCenter(glm::vec3 center);
	void setWorldUp(glm::vec3 worldUp);


};