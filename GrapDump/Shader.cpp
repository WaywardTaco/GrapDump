
#include "Shader.hpp"
#include <iostream>

Shader::Shader(std::string vertex_shader_source_path, std::string fragment_shader_source_path) {
    /* Read the source files into character pointers */
    std::fstream
        vertex_file(vertex_shader_source_path),
        fragment_file(fragment_shader_source_path);

    std::stringstream vertex_stream, fragment_stream;

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    std::string
        vertex_string = vertex_stream.str(),
        fragment_string = fragment_stream.str();

    const char
        *vertex_source = vertex_string.c_str(),
        *fragment_source = fragment_string.c_str();

    /* Initialize shaders */
    GLuint
        vertex_shader = glCreateShader(GL_VERTEX_SHADER),
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    
    this->shader_program = glCreateProgram();

    /* Compile Shaders */
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    /* Link Vertex and Fragment Shaders for use */
    glAttachShader(this->shader_program, vertex_shader);
    glAttachShader(this->shader_program, fragment_shader);
    glLinkProgram(this->shader_program);

    /* Cleanup */
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::use() {
    glUseProgram(this->shader_program);
}

void Shader::passFloat(const char* var_name, float value) {
    glUseProgram(this->shader_program);
    unsigned int address = glGetUniformLocation(this->shader_program, var_name);
    glUniform1f(address, value);
}

void Shader::passBool(const char* var_name, bool value) {
    glUseProgram(this->shader_program);
    unsigned int address = glGetUniformLocation(this->shader_program, var_name);
    glUniform1i(address, value);
}

void Shader::passVec3(const char* var_name, glm::vec3 value) {
    glUseProgram(this->shader_program);
    unsigned int address = glGetUniformLocation(this->shader_program, var_name);
    glUniform3fv(address, 1, glm::value_ptr(value));
}

void Shader::passMat4(const char* var_name, glm::mat4 value) {
    glUseProgram(this->shader_program);
    unsigned int address = glGetUniformLocation(this->shader_program, var_name);
    glUniformMatrix4fv(address, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::pass2DTexture(const char* var_name, GLuint texture, int tex_index) {
    glUseProgram(this->shader_program);
    if (tex_index == 0)
        glActiveTexture(GL_TEXTURE0);
    else if (tex_index == 1)
        glActiveTexture(GL_TEXTURE1);
    else
        return;
    GLuint address = glGetUniformLocation(this->shader_program, var_name);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(address, tex_index);
}

GLuint Shader::getProgram() {
    return this->shader_program;
}