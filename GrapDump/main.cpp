
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
    mouseY = 0.f,
    mouseX = 0.f,
    modelRotSpd = 10.f,
    lightRotSpd = 10.f,
    brightnessStep = 10.f,
    camPanSpeed = 0.5f;

glm::vec3 lightColors[] = {
    {1.f, 1.f, 1.f},
    {1.f, 0.f, 0.f},
    {1.f, 1.f, 0.f},
    {0.f, 1.f, 0.f},
    {0.f, 1.f, 1.f},
    {0.f, 0.f, 1.f},
    {1.f, 0.f, 1.f}
};
int lightColIndex = 0;

float
    window_height = 1000,
    window_width = 1000;

bool
    inOrthoView = false,
    controllingLight = false,
    wPressed = false,
    aPressed = false,
    sPressed = false,
    dPressed = false,
    qPressed = false,
    ePressed = false,
    upPressed = false,
    downPressed = false,
    leftPressed = false,
    rightPressed = false,
    onePressed = false,
    twoPressed = false,
    spacePressed = false,
    mouseUp = false,
    mouseDown = false,
    mouseLeft = false,
    mouseRight = false;

/**********************************************************************/

GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xPos, double yPos);
void processInput(Camera** cam, PerspectiveCamera* perspectiveCam, OrthoCamera* orthoCam, Model* mainModel, Model* light, PointLight* pointLight, DirectionLight* dirLight);
void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv);

/* BUGS / TODOS
    Main object movement
        - Own OBJ w/ Textures
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

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorCallback);


    /* Light declaration */
    PointLight* pointLight = new PointLight({ -15.f, 3.f, -5.f });
    DirectionLight* directionLight = new DirectionLight({-4.f, 5.f, 0.f});

    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
    OrthoCamera* orthoCamera = new OrthoCamera();
    orthoCamera->setPosition({0.01f, 1.f, 0.f});
    Camera* currentCamera = perspectiveCamera;

    Model* mainModel = new Model("3D/djSword.obj", "3D/partenza.jpg");
    Model* lightModel = new Model("3D/djSword.obj", glm::vec3{ 1.f, 1.f, 1.f });

    /*
    Skybox* sky = new Skybox(
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png");
    */











    // Front-back texture fixing
    glEnable(GL_DEPTH_TEST); 

    GLuint shaderProg = compShaderProg("Shader/sample.vert", "Shader/sample.frag");
    GLuint skyboxShader = compShaderProg("Shader/Skybox.vert", "Shader/Skybox.frag");

    
    perspectiveCamera->setPosition({0.f, 0.f, -1.f});
    mainModel->setScale(0.01f);
    lightModel->setScale(0.01f);
    lightModel->setPosition({-0.5f, 0.5f, 0.f});

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Lighting */
        pointLight->apply(shaderProg);
        
        // sky->render(skyboxShader, (Camera*)perspectiveCamera);
        currentCamera->apply(shaderProg);
        mainModel->render(shaderProg);
        lightModel->render(shaderProg);
        
        /* Render Frame and Wait for inputs, then resets window & depth buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        processInput(&currentCamera, perspectiveCamera, orthoCamera, mainModel, lightModel, pointLight, directionLight);
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
        wPressed = true;
    if (key == GLFW_KEY_S)
        sPressed = true;
    if (key == GLFW_KEY_A)
        aPressed = true;
    if (key == GLFW_KEY_D)
        dPressed = true;
    if (key == GLFW_KEY_E)
        ePressed = true;
    if (key == GLFW_KEY_Q)
        qPressed = true;
       
    if (key == GLFW_KEY_SPACE) 
        spacePressed = true;

    if (key == GLFW_KEY_UP)
        upPressed = true;
    if (key == GLFW_KEY_DOWN)
        downPressed = true;
    if (key == GLFW_KEY_RIGHT)
        rightPressed = true;
    if (key == GLFW_KEY_LEFT)
        leftPressed = true;

    if (key == GLFW_KEY_1)
        onePressed = true;
    if (key == GLFW_KEY_2)
        twoPressed = true;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {

    if (ypos > mouseY)
        mouseUp = true;
    if (ypos < mouseY)
        mouseDown = true;
    if (xpos > mouseX)
        mouseRight = true;
    if (xpos < mouseX)
        mouseLeft = true;

    mouseX = xpos;
    mouseY = ypos;
}


void processInput(Camera** cam, PerspectiveCamera* perspectiveCam, OrthoCamera* orthoCam, Model* mainModel, Model* light, PointLight* pointLight, DirectionLight* dirLight) {
    if (wPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 1.f, 0.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 1.f, 0.f, 0.f });
        }
        else
            mainModel->rotate(modelRotSpd, {1.f, 0.f, 0.f});
        wPressed = false;
    }
    if (sPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 1.f, 0.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 1.f, 0.f, 0.f });
        }
        else
            mainModel->rotate(-modelRotSpd, { 1.f, 0.f, 0.f });
        sPressed = false;
    }
    if (dPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 1.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 1.f, 0.f });
        }
        else
            mainModel->rotate(modelRotSpd, { 0.f, 1.f, 0.f });
        dPressed = false;
    }
    if (aPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 1.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 1.f, 0.f });
        }
        else
            mainModel->rotate(-modelRotSpd, { 0.f, 1.f, 0.f });
        aPressed = false;
    }
    if (ePressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 0.f, 1.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 0.f, 1.f });
        }
        else
            mainModel->rotate(modelRotSpd, { 0.f, 0.f, 1.f });
        ePressed = false;
    }
    if (qPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 0.f, 1.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 0.f, 1.f });
        }
        else
            mainModel->rotate(-modelRotSpd, { 0.f, 0.f, 1.f });
        qPressed = false;
    }
    if (upPressed) {
        pointLight->adjustBrightness(brightnessStep);
        upPressed = false;
    }
    if (downPressed) {
        pointLight->adjustBrightness(-brightnessStep);
        downPressed = false;
    }
    if (rightPressed) {
        dirLight->adjustBrightness(brightnessStep);
        rightPressed = false;
    }
    if (leftPressed) {
        dirLight->adjustBrightness(-brightnessStep);
        leftPressed = false;
    }
    if (onePressed) {
        // Cam is set to Perspective
        *cam = perspectiveCam;
        inOrthoView = false;
        onePressed = false;
    }
    if (twoPressed) {
        // Cam is set to ortho
        *cam = orthoCam;
        inOrthoView = true;
        twoPressed = false;
    }
    if (spacePressed) {
        if (controllingLight)
            controllingLight = false;
        else
            controllingLight = true;

        lightColIndex = (lightColIndex + 1) % 7;
        pointLight->setColor(lightColors[lightColIndex]);
        light->setBaseColor(lightColors[lightColIndex]);
        spacePressed = false;
    }
    if (inOrthoView)
        return;

    if (mouseUp) {
        (*cam)->rotateAround(camPanSpeed, { 1.f, 0.f, 0.f });
        mouseUp = false;
    }
    if (mouseDown) {
        (*cam)->rotateAround(-camPanSpeed, { 1.f, 0.f, 0.f });
        mouseDown = false;
    }
    if (mouseRight) {
        (*cam)->rotateAround(camPanSpeed, { 0.f, 1.f, 0.f });
        mouseRight = false;
    }
    if (mouseLeft) {
        (*cam)->rotateAround(-camPanSpeed, { 0.f, 1.f, 0.f });
        mouseLeft = false;
    }
}

void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv) {
    unsigned int varLoc = glGetUniformLocation(shaderProg, variable);
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(matrix4fv));
}

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