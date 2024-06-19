
#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "../tiny_obj_loader.h"
#endif

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../stb_image.h"
#endif

class Shader {
private:
	GLuint shader_program;

public:
	Shader(std::string vertex_shader_source_path, std::string fragment_shader_source_path);
	void use();
	void passFloat(const char* var_name, float value);
	void passBool(const char* var_name, bool value);
	void passVec3(const char* var_name, glm::vec3 value);
	void passMat4(const char* var_name, glm::mat4 value);
	void pass2DTexture(const char* var_name, GLuint texture, int tex_index);
	GLuint getProgram();
};