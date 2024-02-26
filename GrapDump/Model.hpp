#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Model {
private:
    bool success;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    GLuint VAO, VBO, EBO;
    std::vector<GLuint> mesh_indices;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

public:
    Model(std::string objectPath);
    ~Model();

    void render(GLuint shaderProgram);
    void setPosition(glm::vec3 position);
    void setScale(glm::vec3 scale);
    void setRotation(glm::vec3 rotation);
};
