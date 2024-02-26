#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tiny_obj_loader.h"
#include "stb_image.h"

class Model {
private:
    bool success;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    GLuint VAO, VBO, texture;
    std::vector<GLfloat> fullVertexData;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 modelScale;

public:
    Model(std::string modelPath, std::string texturePath);
    ~Model();

public:
    void render(GLuint shaderProgram);

    void move(glm::vec3 position);
    void rotate(glm::vec3 rotation);
    void scale(float scale);
    void scale(glm::vec3 scale);

    void setPosition(glm::vec3 position);
    void setRotation(glm::vec3 rotation);
    void setScale(float scale);
    void setScale(glm::vec3 scale);
};
