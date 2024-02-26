
#include "Model.hpp"

Model::Model(std::string objectPath) : position(0.f, 0.f, 0.f), scale(1.f, 1.f, 1.f), rotation(0.f, 0.f, 0.f) {

    this->success = tinyobj::LoadObj(
        &this->attributes,
        &this->shapes,
        &this->material,
        &this->warning,
        &this->error,
        objectPath.c_str()
    );

    for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {
        this->mesh_indices.push_back(this->shapes[0].mesh.indices[i].vertex_index);
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * this->attributes.vertices.size(), this->attributes.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glEnableVertexAttribArray(0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh_indices.size(), this->mesh_indices.data(), GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


};

Model::~Model() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Model::render(GLuint shaderProgram) {

    glm::mat4 transformMat(1.f);

    transformMat = glm::translate(transformMat, this->position);

    transformMat = glm::rotate(transformMat, glm::radians(this->rotation.x), glm::vec3(1.f,0.f,0.f));
    transformMat = glm::rotate(transformMat, glm::radians(this->rotation.y), glm::vec3(0.f,1.f,0.f));
    transformMat = glm::rotate(transformMat, glm::radians(this->rotation.z), glm::vec3(0.f,0.f,1.f));

    transformMat = glm::scale(transformMat, this->scale);
    

    unsigned int varLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(transformMat));

    glUseProgram(shaderProgram);
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->mesh_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

void Model::setPosition(glm::vec3 position) {
    this->position = position;
};

void Model::setScale(glm::vec3 scale) {
    this->scale = scale;
}

void Model::setRotation(glm::vec3 scale) {
    this->rotation = rotation;
}

