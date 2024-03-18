
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
#include <vector>
#include <iostream>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "tiny_obj_loader.h"
#endif

#include "Model.hpp"
#include "Camera.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthoCamera.hpp"
#include "Skybox.hpp"
#include "LightSource.hpp"
#include "PointLight.hpp"
#include "DirectionLight.hpp"

float
    x_mov = 0.f,
    y_mov = 1.f,
    x_rot = 0.f,
    y_rot = 0.f,
    s_scale = 1.f,
    z_zoom = 0.f
;

float
    window_height = 1000,
    window_width = 1000;

bool controllingLight = false;

/**********************************************************************/

GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv);

/* BUGS / TODOS
    Main object movement
        - Own OBJ w/ Textures
        AD - Y Axis
        WS - X Axis
        QE - Z Axis
        Space - swap to light control (change light color)
    Light Model
        - Unlit but colored to light color for point light
        - Direction Light at {4, -5, 0} pointing to center
        AD - around the y
        WS - around the x
        QE - around the z
        Space - swap to main object control (change color)
        Up/Down - point light brightness
        Left/Right - direction light brightness
        - Direction light not tested yet
        - Light shading doesnt handle multiple lightsources yet
    POV Camera
        - mouse rotates around the main model
        - 2 for ortho
    Ortho Camera
        - sees point light and model
        - 1 for perspective
        - OrthoCamera looks weird when plugged in

*/


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


    /* Light declaration */
    PointLight pointLight({ -15.f, 3.f, -5.f });
    DirectionLight directionLight({-4.f, 5.f, 0.f});

    Camera* currentCamera;
    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
    OrthoCamera* orthoCamera = new OrthoCamera();

    Model mainModel("3D/djSword.obj", "3D/partenza.jpg");
    Model lightModel("3D/djSword.obj", glm::vec3{ 0.5f, 0.5f, 0.f });

    Skybox* sky = new Skybox(
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png");











    // Front-back texture fixing
    glEnable(GL_DEPTH_TEST); 

    GLuint shaderProg = compShaderProg("Shader/sample.vert", "Shader/sample.frag");
    GLuint skyboxShader = compShaderProg("Shader/Skybox.vert", "Shader/Skybox.frag");

    
    perspectiveCamera->setPosition({0.f, 0.f, -1.f});
    mainModel.setScale(0.01f);
    lightModel.setScale(0.01f);
    lightModel.setPosition({-0.5f, 0.5f, 0.f});

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Lighting */
        pointLight.apply(shaderProg);
        
        sky->render(skyboxShader, (Camera*)perspectiveCamera);
        perspectiveCamera->apply(shaderProg);
        mainModel.render(shaderProg);
        lightModel.render(shaderProg);
        
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

    if (key == GLFW_KEY_W)
        if(controllingLight)
            std::cout << "Rotate X Up" << std::endl;
        else
            std::cout << "X Up" << std::endl;
    if (key == GLFW_KEY_S)
        if (controllingLight)
            std::cout << "Rotate X Down" << std::endl;
        else
            std::cout << "X Down" << std::endl;
    if (key == GLFW_KEY_A)
        if (controllingLight)
            std::cout << "Rotate Y Up" << std::endl;
        else
            std::cout << "Y Up" << std::endl;
    if (key == GLFW_KEY_D)
        if (controllingLight)
            std::cout << "Rotate Y Down" << std::endl;
        else
            std::cout << "Y Down" << std::endl;
    if (key == GLFW_KEY_E)
        if (controllingLight)
            std::cout << "Rotate Z Up" << std::endl;
        else
            std::cout << "Z Up" << std::endl;
    if (key == GLFW_KEY_Q)
        if (controllingLight)
            std::cout << "Rotate Z Down" << std::endl;
        else
            std::cout << "Z Down" << std::endl;
       
    if (key == GLFW_KEY_SPACE) {
        if (controllingLight)
            controllingLight = false;
        else
            controllingLight = true;

        std::cout << "Light shifts color" << std::endl;
    }

    if (key == GLFW_KEY_UP)
        std::cout << "Point light brighter" << std::endl;
    if (key == GLFW_KEY_DOWN)
        std::cout << "Point light down" << std::endl;
    if (key == GLFW_KEY_RIGHT)
        std::cout << "Direction Light Up" << std::endl;
    if (key == GLFW_KEY_LEFT)
        std::cout << "Direction Light Down" << std::endl;

    if (key == GLFW_KEY_1)
        std::cout << "Current cam set to perspective" << std::endl;
    if (key == GLFW_KEY_2)
        std::cout << "Current cam set to ortho" << std::endl;
    
    /* POV Drag movement */
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

    /*Renders model with reference to an imaginary window with reference to the actul window
       glViewport(
           0,      // Minimum x
           0,      // Minimum y
           1200,    // Max x, width
           600     // Max y, height
       );
       */