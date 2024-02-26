
#include "Model.hpp"

Model::Model(std::string modelPath, std::string texturePath) :
    position(0.f), modelScale(1.f), rotation(0.f)
{

    this->success = tinyobj::LoadObj(
        &this->attributes,
        &this->shapes,
        &this->material,
        &this->warning,
        &this->error,
        modelPath.c_str()
    );

    for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {

        tinyobj::index_t vData = this->shapes[0].mesh.indices[i];

        /* Load X, Y, Z, Nx, Ny, Nz, U, and V data */
        this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3)]);
        this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 1]);
        this->fullVertexData.push_back(this->attributes.vertices[(vData.vertex_index * 3) + 2]);

        this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3)]);
        this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 1]);
        this->fullVertexData.push_back(this->attributes.normals[(vData.normal_index * 3) + 2]);

        this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2)]);
        this->fullVertexData.push_back(this->attributes.texcoords[(vData.texcoord_index * 2) + 1]);
    }

    /* Generate VAO & VBO and start editing */
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    /* Load data into VBO then VAO */
    /* Type of Buffer Data | Size of array to copy | Array to copy | For render optimization */
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * this->fullVertexData.size(), this->fullVertexData.data(), GL_DYNAMIC_DRAW);

    /* Normals and UV Offset for data loading */
    GLintptr normalsPtr = 3 * sizeof(float);
    GLintptr uvPtr = 6 * sizeof(float);

    /* Data Layer | Component Count for X, Y, Z | Data Type | If Normalized | Size of a Vertex (w/ UV) | Stride value (offset) */
    /* 0 is Pos, 2 is UV */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)normalsPtr);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    /* Texture Image Prep */
    int img_width, img_height,
        colorChannels; // Count of r,g,b, & a

    // Fix flipped image
    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load(texturePath.c_str(),
            &img_width,
            &img_height,
            &colorChannels,
            0);  // Boarder

    /* Texture Gen */
    glGenTextures(1, &this->texture); // 1 is count of textures
    glActiveTexture(GL_TEXTURE0); // Preloads textures at index 0, multiple indecies for simultaneous rendering
    glBindTexture(GL_TEXTURE_2D, this->texture);  // Binds textures to be generated to index 0 which was preloaded

    if (colorChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D,
            0,  // Texture 0
            GL_RGBA, // Target color format of texture GL_RGB if only 3 color channels, FROM RGBA
            img_width,
            img_height,
            0,  // Boarder
            GL_RGBA, // Color format of the texure, TO RGBA, can differ frm FROM but needs processing
            GL_UNSIGNED_BYTE,
            tex_bytes); // Loaded texture a while ago
    }
    else if (colorChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D,
            0,  // Texture 0
            GL_RGB, // Target color format of texture GL_RGB if only 3 color channels, FROM RGBA
            img_width,
            img_height,
            0,  // Boarder
            GL_RGB, // Color format of the texure, TO RGBA, can differ frm FROM but needs processing
            GL_UNSIGNED_BYTE,
            tex_bytes); // Loaded texture a while ago

    }


    /* Mipmaps: Smaller versions of a loaded texture for optimized lowres versions of image at a distance */
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes); // Free the previously loaded textures

    /* Clear Editing VAO and VBO */
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};

Model::~Model() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Model::render(GLuint shaderProgram) {
    glm::mat4 transform(1.f);

    transform = glm::translate(transform, this->position);
    transform = glm::scale(transform, this->modelScale);

    transform = glm::rotate(transform, glm::radians(this->rotation.x), { 1.f, 0.f, 0.f });
    transform = glm::rotate(transform, glm::radians(this->rotation.y), { 0.f, 1.f, 0.f });
    transform = glm::rotate(transform, glm::radians(this->rotation.z), { 0.f, 0.f, 1.f });

    unsigned int varLoc = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(transform));

    /* Texture stuff */
    GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Address, 0); // 0 is the index of the texture

    /* Render VAO w/ shader */
    glUseProgram(shaderProgram); // Applies shaders to all follwoing vertecies
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 5);
    glBindVertexArray(0);
};

void Model::move(glm::vec3 position) {
    this->position.x += position.x;
    this->position.y += position.y;
    this->position.z += position.z;
};

void Model::rotate(glm::vec3 rotation) {
    this->rotation.x += rotation.x;
    this->rotation.y += rotation.y;
    this->rotation.z += rotation.z;
};

void Model::scale(float scale) {
    this->modelScale.x += scale;
    this->modelScale.y += scale;
    this->modelScale.z += scale;
};

void Model::scale(glm::vec3 scale) {
    this->modelScale.x += scale.x;
    this->modelScale.y += scale.y;
    this->modelScale.z += scale.z;
};

void Model::setPosition(glm::vec3 position) {
    this->position = position;
};

void Model::setRotation(glm::vec3 rotation) {
    this->rotation = rotation;
};

void Model::setScale(float scale) {
    this->modelScale = glm::vec3(scale);
};

void Model::setScale(glm::vec3 scale) {
    this->modelScale = scale;
};
