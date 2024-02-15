
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <iostream>

#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float
    x_mov = 0.f,
    y_mov = 1.f,
    x_rot = 0.f,
    y_rot = 0.f,
    s_scale = 1.f,
    z_zoom = 0.f
;

// Manually Defined for Cube for now
GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
};

float
    window_height = 640,
    window_width = 640;

class LinTransform {
public:
    glm::mat4 matrix;

public:
    LinTransform();

public:
    void transform(float x, float y, float z);
    void transform(glm::vec3 relLocation);
    void scale(float mult);
    void scale(float x, float y, float z);
    void scale(glm::vec3 scaleVector);
    void rotate(float degrees, float x, float y, float z);
    void rotate(float degrees, glm::vec3 rotationAxis);
    void reset();
};

class Model {
private:
    bool success;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    GLuint VAO, VBO, EBO, VBO_UV, texture;
    std::vector<GLuint> mesh_indices;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Model(std::string path);
    ~Model();

    void render();

    GLuint getTexture();
};


GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv);

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Josiah Aviso", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    /* Initializations */
    gladLoadGL();
    glfwSetKeyCallback(window, keyCallback);

    // Front-back texture fixing
    glEnable(GL_DEPTH_TEST); 

    /*Renders model with reference to an imaginary window with reference to the actul window
    glViewport(
        0,      // Minimum x
        0,      // Minimum y
        1200,    // Max x, width
        600     // Max y, height
    );
    */

    GLuint shaderProg = compShaderProg("Shader/sample.vert", "Shader/sample.frag");

    // Matrix to modify the projection style
    glm::mat4 orthProjectionMatrix = glm::ortho(
        -2.f,   // Leftmost Point
        2.f,    // Rightmost Point
        -2.f,   // Bottom Point
        2.f,    // Top Point
        -1.f,   // Znear
        1.f     // Zfar
    );

    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(100.f), // FOV
        window_height / window_width, // aspect ratio
        0.1f, //Znear > 0
        100.f //Zfar
    );

    Model model("3D/myCube.obj");
    Model model2("3D/bunny.obj");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
       

                
        glm::vec3 cameraPos = glm::vec3(x_mov, z_zoom, -y_mov);

        glm::vec3 cameraCenter = glm::vec3(0.f, 0.f, 0.f);

        glm::mat4 cameraMatrix = glm::lookAt(cameraPos, cameraCenter, glm::vec3(0.f, 1.f, 0.f));
        
        setShaderMat4fv(shaderProg, "view", cameraMatrix);
        setShaderMat4fv(shaderProg, "projection", projectionMatrix);

        
        
        
        // Texture Stuff
        GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
        glBindTexture(GL_TEXTURE_2D, model.getTexture());
        glUniform1i(tex0Address, 0); // 0 is the index of the texture
        
        

        LinTransform transformMat;



        // DRAW Code for Model 1
        
        transformMat.transform(0.f,0.f,0.f);
        transformMat.scale(0.2f);
        setShaderMat4fv(shaderProg, "transform", transformMat.matrix);

        glUseProgram(shaderProg); // Applies shaders to all follwoing vertecies
        
        model.render();  
        
        // DRAW Code for Model 2...

        

        transformMat.transform(2.f, 2.f, 2.f);
        setShaderMat4fv(shaderProg, "transform", transformMat.matrix);
        glUseProgram(shaderProg); // Applies shaders to all follwoing vertecies
        model2.render();
        

        /* Render Frame and Wait for inputs */
        glfwSwapBuffers(window);
        glfwPollEvents();

        /* Resets the window to clear to prevent artifacts & resets the depth buffer */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    /* Cleaning of Objects */
    glfwTerminate();

    return 0;
}

LinTransform::LinTransform() : matrix(1.f) {};
void LinTransform::transform(float x, float y, float z) { this->transform(glm::vec3(x, y, z)); };
void LinTransform::transform(glm::vec3 relLocation) { this->matrix = glm::translate(this->matrix, relLocation); };
void LinTransform::scale(float mult) { this->scale(mult, mult, mult); };
void LinTransform::scale(float x, float y, float z) { this->scale(glm::vec3(x, y, z)); };
void LinTransform::scale(glm::vec3 scaleVector) { this->matrix = glm::scale(this->matrix, scaleVector); };
void LinTransform::rotate(float degrees, float x, float y, float z) { this->rotate(degrees, glm::vec3(x, y, z)); };
void LinTransform::rotate(float degrees, glm::vec3 rotationAxis) { 
    this->matrix = glm::rotate(this->matrix, glm::radians(degrees), glm::normalize(rotationAxis));
};
void LinTransform::reset() { this->matrix = glm::mat4(1.f); };

Model::Model(std::string path) {

    this->success = tinyobj::LoadObj(
        &this->attributes,
        &this->shapes,
        &this->material,
        &this->warning,
        &this->error,
        path.c_str()
    );

    for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {
        this->mesh_indices.push_back(this->shapes[0].mesh.indices[i].vertex_index);
    }

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    // Currently editing VAO = null
    glBindVertexArray(this->VAO);
    // Currently editing VAO = VAO variable

    // Currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // VAO variable <- VBO variable *not null <- VBO variable
    // Currently editing VBO = VBO variable

    glBufferData(
        GL_ARRAY_BUFFER,    // Type of data in the buffer
        sizeof(GL_FLOAT) * this->attributes.vertices.size(),   // Size of the array
        this->attributes.vertices.data(), // Array of points, feeds the whole array *Note: can also do &attributes.vertices[0]
        GL_STATIC_DRAW      // For optimizing the render
    );

    glVertexAttribPointer(
        0,                  // Index, also type of data, 0 is positional data, 2 is textures, etc...
        3,                  // Number of components (x, y, z)
        GL_FLOAT,           // Data type of compontents
        GL_FALSE,           // If data is normalized or not
        3 * sizeof(float),  // Byte size of each vertex
        (void*)0            // ???
    );

    glEnableVertexAttribArray(0); // Index 0 used since prev buffer is declared to index 0

    // Binds currently editing buffer to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * this->mesh_indices.size(),
        this->mesh_indices.data(),
        GL_STATIC_DRAW
    );

    /* Texture Image Prep */
    int img_width, img_height, colorChannels; // Count of r,g,b, & a

    // Fix flipped image
    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes =
        stbi_load("3D/ayaya.png",
            &img_width,
            &img_height,
            &colorChannels,
            0);  // Boarder

    /* Texture Gen */
    glGenTextures(1, &this->texture); // 1 is count of textures
    glActiveTexture(GL_TEXTURE0); // Preloads textures at index 0, multiple indecies for simultaneous rendering
    glBindTexture(GL_TEXTURE_2D, this->texture);  // Binds textures to be generated to index 0 which was preloaded

    glTexImage2D(GL_TEXTURE_2D,
        0,  // Texture 0
        GL_RGBA, // Target color format of texture GL_RGB if only 3 color channels, FROM RGBA
        img_width,
        img_height,
        0,  // Boarder
        GL_RGBA, // Color format of the texure, TO RGBA, can differ frm FROM but needs processing
        GL_UNSIGNED_BYTE,
        tex_bytes); // Loaded texture a while ago

    /* Mipmaps: Smaller versions of a loaded texture for optimized lowres versions of image at a distance */
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes); // Free the previously loaded textures

    glGenBuffers(1, &this->VBO_UV);


    /* UV Prep Stuff */
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_UV);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])),   // size of array
        &UV[0], // Array itself of UVs
        GL_DYNAMIC_DRAW);
    glVertexAttribPointer(
        2, // For Texture Data
        2, // 2 components since UVs
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float), // Size of a UV
        (void*)0);
    glEnableVertexAttribArray(2);


    // currently editing VBO = VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // currently editing VBO = null

    // currently editing VAO = VAO variable
    glBindVertexArray(0);
    // currently editing VAO = null

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
};

Model::~Model() {

    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Model::render() {
    glBindVertexArray(this->VAO);

    glDrawElements(GL_TRIANGLES, this->mesh_indices.size(), GL_UNSIGNED_INT,
        0 // Start at index 0
    );

    glBindVertexArray(0);
};

GLuint Model::getTexture() {
    return this->texture;
};


GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc) {
    
    /* Read the Shaders from their source files */
    std::fstream        
                        vertFile(vertShaderSrc),
                        fragFile(fragShaderSrc);
    std::stringstream   
                        vertBuff,
                        fragBuff;
                        
                        vertBuff << vertFile.rdbuf();
                        fragBuff << fragFile.rdbuf();
    std::string         
                        vertStr = vertBuff.str(),
                        fragStr = fragBuff.str();
    const char          
                        *v = vertStr.c_str(),
                        *f = fragStr.c_str();
    GLuint
                        vertexShader = glCreateShader(GL_VERTEX_SHADER),
                        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER),
                        shaderProg = glCreateProgram();

    /* Compile Shaders */
    glShaderSource(vertexShader, 1, &v, NULL);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    /* Link Vertex and Fragment Shaders for use */
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader); 
    glLinkProgram(shaderProg);

    return shaderProg;
};

void keyCallback(
    GLFWwindow* window,     // Pointer to the window being checked
    int key,                // the keycode being pressed
    int scancode,           // Physical position of the press on keyboard
    int action,             // Either pressed or released
    int mods                // Which modifier keys are held (like Shift, Ctrl, Alt, etc.)
)
{
    if (action == GLFW_RELEASE)
        return;

    if (key == GLFW_KEY_W && action != GLFW_RELEASE)
        y_mov -= 0.02;
    if (key == GLFW_KEY_A && action != GLFW_RELEASE)
        x_mov += 0.02;
    if (key == GLFW_KEY_S && action != GLFW_RELEASE)
        y_mov += 0.02;
    if (key == GLFW_KEY_D && action != GLFW_RELEASE)
        x_mov -= 0.02;

    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        z_zoom += 0.02;
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        z_zoom -= 0.02;
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        x_rot -= 5;
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        x_rot += 5;

    if (key == GLFW_KEY_Q && action != GLFW_RELEASE)
        s_scale -= 0.02;
    if (key == GLFW_KEY_E && action != GLFW_RELEASE)
        s_scale += 0.02;

    if (key == GLFW_KEY_Z && action != GLFW_RELEASE)
        z_zoom += 0.02;
    if (key == GLFW_KEY_X && action != GLFW_RELEASE)
        z_zoom -= 0.02;

};

void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv) {
    unsigned int varLoc = glGetUniformLocation(shaderProg, variable);
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(matrix4fv));
};


/* Older Notes */
/*
glm::mat4 getViewMatrix(glm::vec3 cameraPos, glm::vec3 center) {

    glm::vec3 WorldUp = glm::vec3(0.f, 1.f, 0.f);

    glm::mat4 eyePosition = glm::translate(glm::mat4(1.f),
        cameraPos * -1.f);  // times -1 since matrix takes negative position to transform the world

    glm::vec3 forwardVec = glm::normalize(glm::vec3(center - cameraPos));
    glm::vec3 rightVec = glm::normalize(glm::cross(forwardVec, WorldUp));
    glm::vec3 upVec = glm::normalize(glm::cross(rightVec, forwardVec));

    glm::mat4 eyeOrient = glm::mat4(1.f);

    // [Col][Row]
    eyeOrient[0][0] = rightVec.x;
    eyeOrient[1][0] = rightVec.y;
    eyeOrient[2][0] = rightVec.z;

    eyeOrient[0][1] = upVec.x;
    eyeOrient[1][1] = upVec.y;
    eyeOrient[2][1] = upVec.z;

    eyeOrient[0][2] = - forwardVec.x;
    eyeOrient[1][2] = - forwardVec.x;
    eyeOrient[2][2] = - forwardVec.x;

    glm::mat4 viewMatrix = eyeOrient * eyePosition;

    return viewMatrix;
};
*/


    // Sample Code
    // unsigned int xLoc = glGetUniformLocation(shaderProg, "x"); // Grabs variable named 'x' from the shader
    // glUniform1f(xLoc, x_mod); // Assigns one float from x_mod to xLoc which refers to x in the shader
    // unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
    // glUniform1f(yLoc, y_mod);

    /*
    glDrawArrays(
        GL_TRIANGLES,       // Type of primitives drawn
        0,                  // Start from index 0
        3                   // Number of components
    );
    */