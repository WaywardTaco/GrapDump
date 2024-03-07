#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	double centerDist;

	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 worldUp;

	glm::mat4 projection;

public:
	Camera(glm::mat4 projection);

public:
	void apply(GLuint shaderProgram);

	void adjustCenterDist(double adjust);
	void absMove(glm::vec3 move);
	void relMove(glm::vec3 move);
	void absRotate(glm::vec3 rotate);
	void relRotate(glm::vec3 rotate);
	void absMoveCenter(glm::vec3 move);
	void relMoveCenter(glm::vec3 move);

	void setCenterDist(double centerDist);
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setCenter(glm::vec3 center);
	void setWorldUp(glm::vec3 worldUp);
	void setProjection(glm::mat4 projection);

	glm::vec3 getRotation();

};