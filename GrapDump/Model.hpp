#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "tiny_obj_loader.h"
#endif

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

class Model {
private:
    bool success;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    GLuint VAO, VBO, texture, normalMap;
    std::vector<GLfloat> fullVertexData;

    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    glm::vec3 position;
    glm::vec3 modelScale;
    glm::mat4 rotation;

    glm::vec3 modelBaseColor;
    bool 
        hasTexture,
        hasNormMap;

public:
    Model(std::string modelPath, std::string texturePath);
    Model(std::string modelPath, glm::vec3 plainColor);
    Model(std::string modelPath, std::string texturePath, std::string normalsMapPath);
    ~Model();

public:
    void render(GLuint shaderProgram);

    void move(glm::vec3 position);
    void rotate(float degrees, glm::vec3 axis);
    void scale(float scale);
    void scale(glm::vec3 scale);

    void setPosition(glm::vec3 position);
    void rotateAround(glm::vec3 center, float degrees, glm::vec3 axis);
    void setRotation(float degrees, glm::vec3 axis);
    void setScale(float scale);
    void setScale(glm::vec3 scale);
    void setBaseColor(glm::vec3 color);
};