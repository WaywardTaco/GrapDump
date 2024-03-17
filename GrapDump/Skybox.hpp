
#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

#include "Camera.hpp"

class Skybox {
private:
	unsigned int 
		VAO, VBO, EBO, texture;

public:
	Skybox(
		std::string rightTex,
		std::string leftTex,
		std::string upTex,
		std::string downTex,
		std::string frontTex,
		std::string backTex);

	void render(GLuint shaderProgram, Camera* camera);
};