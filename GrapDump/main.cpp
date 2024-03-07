
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <iostream>

#include "Model.hpp"
#include "Camera.hpp"

float
    x_mov = 0.f,
    y_mov = 1.f,
    x_rot = 0.f,
    y_rot = 0.f,
    s_scale = 1.f,
    z_zoom = 0.f
;

float
    window_height = 640,
    window_width = 640;

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
    GLuint skyboxShader = compShaderProg("Shader/Skybox.vert", "Shader/Skybox.frag");

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

    Model model("3D/djSword.obj", "3D/partenza.jpg");

    /* Light declaration */
    glm::vec3 lightPos = glm::vec3(-15.f, 3.f, 0.f);
    glm::vec3 lightColor = glm::vec3(1.f, 1.f, 1.f);

    float ambientStr = 0.1f;
    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;
    float specPhong = 16;

    float brightness = 150.f;

    /* Skybox stuff */
    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    // Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    // Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    // Skybox Textures
    std::string skyboxFaces[]{
        "Skybox/rainbow_rt.png", // Right
        "Skybox/rainbow_lf.png", // Left
        "Skybox/rainbow_up.png", // Up
        "Skybox/rainbow_dn.png", // Down
        "Skybox/rainbow_ft.png", // Front
        "Skybox/rainbow_bk.png"  // Back
    };

    /* Skybox Loading */
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Skybox Textures */
    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    
    // Prevents skybox pixelation when texture is magnified or minimized
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Ensures Textures are stretched to edge (R = x, S = z, T = y)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(skyboxFaces[i].c_str(), &w, &h, &skyCChannel, 0);
        if (data) {

            if (skyCChannel == 3) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, // Order is Right, Left, Up, Down, Front, Back
                    0,
                    GL_RGB,
                    w,
                    h,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data
                );
            } else if (skyCChannel == 4) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGBA,
                    w,
                    h,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    data
                );
            }

            stbi_image_free(data);
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glm::vec3 cameraPos = glm::vec3(x_mov, z_zoom, -y_mov);

        glm::vec3 cameraCenter = glm::vec3(0.f, 0.f, 0.f);

        glm::mat4 cameraMatrix = glm::lookAt(cameraPos, cameraCenter, glm::vec3(0.f, 1.f, 0.f));
        
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShader);
            
        glm::mat4 skyMatrix = glm::mat4(1.f);
        skyMatrix = glm::mat4(glm::mat3(cameraMatrix));

        setShaderMat4fv(skyboxShader, "view", skyMatrix);
        setShaderMat4fv(skyboxShader, "projection", projectionMatrix);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,0);

        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glUseProgram(shaderProg);

        setShaderMat4fv(shaderProg, "view", cameraMatrix);
        setShaderMat4fv(shaderProg, "projection", projectionMatrix);

        /* Lighting */
        GLuint lightAddress = glGetUniformLocation(shaderProg, "lightPos");
        glUniform3fv(lightAddress, 1, glm::value_ptr(lightPos));
                
        GLuint lightColorAddress = glGetUniformLocation(shaderProg, "lightColor");
        glUniform3fv(lightColorAddress, 1, glm::value_ptr(lightColor));

        GLuint brightnessAddress = glGetUniformLocation(shaderProg, "brightness");
        glUniform1f(brightnessAddress, brightness);

        GLuint ambientStrAddress = glGetUniformLocation(shaderProg, "ambientStr");
        glUniform1f(ambientStrAddress, ambientStr);

        GLuint ambientColorAddress = glGetUniformLocation(shaderProg, "ambientColor");
        glUniform3fv(ambientColorAddress, 1, glm::value_ptr(ambientColor));

        GLuint cameraPosAddress = glGetUniformLocation(shaderProg, "cameraPos");
        glUniform3fv(cameraPosAddress, 1, glm::value_ptr(cameraPos));

        GLuint specStrAddress = glGetUniformLocation(shaderProg, "specStr");
        glUniform1f(specStrAddress, specStr);

        GLuint specPhongAddress = glGetUniformLocation(shaderProg, "specPhong");
        glUniform1f(specPhongAddress, specPhong);


        
        model.setScale(0.01f);
        model.setRotation({0.f, -60.f, 0.f});
        model.render(shaderProg);  

        /* Render Frame and Wait for inputs, then resets window & depth buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

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

    /* Cleanup */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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