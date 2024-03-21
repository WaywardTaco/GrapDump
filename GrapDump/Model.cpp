
#include "Model.hpp"

#define STB_IMAGE_IMPLEMENTATION

Model::Model(std::string modelPath, std::string texturePath) :
    position(0.f), modelScale(1.f), rotation(1.f), modelBaseColor(1.f), hasTexture(true)
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
    stbi_set_flip_vertically_on_load(false);
};

Model::Model(std::string modelPath, glm::vec3 plainColor) :
    position(0.f), modelScale(1.f), rotation(1.f), modelBaseColor(plainColor), hasTexture(false), texture(0)
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

    /* Data Layer | Component Count for X, Y, Z | Data Type | If Normalized | Size of a Vertex (w/ UV) | Stride value (offset) */
    /* 0 is Pos, 2 is UV */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)normalsPtr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* Clear Editing VAO and VBO */
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
};

Model::~Model() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}

void Model::render(GLuint shaderProgram) {
    glUseProgram(shaderProgram); 
    
    glm::mat4 transform(1.f);
    transform = glm::translate(transform, this->position);
    transform = glm::scale(transform, this->modelScale);
    transform = transform * this->rotation;

    unsigned int transformAdrs = glGetUniformLocation(shaderProgram, "transform");
    glUniformMatrix4fv(transformAdrs, 1, GL_FALSE, glm::value_ptr(transform));

    unsigned int baseColorAdrs = glGetUniformLocation(shaderProgram, "modelBaseColor");
    glUniform3fv(baseColorAdrs, 1, glm::value_ptr( this->modelBaseColor));

    unsigned int hasTexAdrs = glGetUniformLocation(shaderProgram, "modelHasTexture");
    glUniform1i(hasTexAdrs, this->hasTexture);

    /* Texture stuff */
    GLuint tex0Adrs = glGetUniformLocation(shaderProgram, "tex0");
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glUniform1i(tex0Adrs, 0); // 0 is the index of the texture

    /* Render VAO w/ shader */
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 5);
    glBindVertexArray(0);
};

void Model::move(glm::vec3 position) {
    this->position.x += position.x;
    this->position.y += position.y;
    this->position.z += position.z;
};

void Model::rotate(float degrees, glm::vec3 axis) {
    this->rotation = glm::rotate(this->rotation, glm::radians(degrees), glm::normalize(glm::vec3(glm::inverse(this->rotation) * glm::vec4(axis,1.f))));
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

void Model::rotateAround(glm::vec3 center, float degrees, glm::vec3 axis) {
    glm::vec3 worldUp = { 0.f, 1.f, 0.f };

    glm::vec3 forwardVec = { 0.f, 0.f, 1.f };
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, worldUp));

    glm::vec3 upVec = glm::normalize(glm::cross(rightVec, forwardVec));

    glm::mat4 cameraTransform = glm::mat4(1.f);

    // [Col][Row]
    cameraTransform[0][0] = rightVec.x;
    cameraTransform[0][1] = rightVec.y;
    cameraTransform[0][2] = rightVec.z;

    cameraTransform[1][0] = upVec.x;
    cameraTransform[1][1] = upVec.y;
    cameraTransform[1][2] = upVec.z;

    cameraTransform[2][0] = forwardVec.x;
    cameraTransform[2][1] = forwardVec.y;
    cameraTransform[2][2] = forwardVec.z;

    glm::vec3 transformedAxis = glm::normalize(glm::vec3(cameraTransform * glm::vec4(glm::normalize(axis), 1.f)));


    glm::mat4 positionTransform = glm::mat4(1.f);
    positionTransform = glm::rotate(positionTransform, glm::radians(degrees), transformedAxis);

    this->position = glm::vec3(positionTransform * glm::vec4(this->position, 1.f));

    // Get the basis vectors centered on the camera center
    // Turn into a basis transform matrix
    // Find the axis of rotation in that new basis
    // Rotate the basis across that axis
    // Apply transform to the relative position of the camera


    // Transform the camera position with respect to the axis of transform of the center (ie. figure out the rotation about the axis in the new vector space)
    // Then apply the same transform as usual to the postion with respect to that axis to the camera
}

void Model::setRotation(float degrees, glm::vec3 axis) {
    this->rotation = glm::rotate(glm::mat4(1.f), glm::radians(degrees), glm::normalize(axis));
};

void Model::setScale(float scale) {
    this->modelScale = glm::vec3(scale);
};

void Model::setScale(glm::vec3 scale) {
    this->modelScale = scale;
};

void Model::setBaseColor(glm::vec3 color) {
    this->modelBaseColor = color;
}