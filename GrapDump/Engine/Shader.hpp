
#pragma once

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine {
	class Shader {
		private:
			GLuint shader_program;

		public:
			Shader(std::string vertex_shader_source_path, std::string fragment_shader_source_path);
			
			void use();
			GLuint getProgram();

			void passData(const char* var_name, float value);
			void passData(const char* var_name, bool value);
			void passData(const char* var_name, glm::vec3 value);
			void passData(const char* var_name, glm::mat4 value);
			void passData(const char* var_name, GLuint texture, int tex_index);
	};
}