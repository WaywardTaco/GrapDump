
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

// STBI include guard
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "stb_image.h"
#endif

// TinyObj include guard
#ifndef INCLUDE_TINYOBJ
#define INCLUDE_TINYOBJ
#include "tiny_obj_loader.h"
#endif

// Object HPPs
#include "Model.hpp"
#include "Camera.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthoCamera.hpp"
#include "Skybox.hpp"
#include "LightSource.hpp"
#include "PointLight.hpp"
#include "DirectionLight.hpp"

// Global Variables
float
    window_height = 1000,
    window_width = 1000,
    mouseY = 0.f,
    mouseX = 0.f,
    modelRotSpd = 10.f,
    lightRotSpd = 10.f,
    brightnessStep = 10.f,
    camPanSpeed = 0.5f;

// Lights for point light
int lightColIndex = 0;
glm::vec3 lightColors[] = {
    {1.f, 1.f, 1.f},
    {1.f, 0.f, 0.f},
    {1.f, 1.f, 0.f},
    {0.f, 1.f, 0.f},
    {0.f, 1.f, 1.f},
    {0.f, 0.f, 1.f},
    {1.f, 0.f, 1.f}
};

// Flags for Input handling
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

// Function Predefs
GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xPos, double yPos);
void processInput(Camera** cam, PerspectiveCamera* perspectiveCam, OrthoCamera* orthoCam, Model* mainModel, Model* light, PointLight* pointLight, DirectionLight* dirLight);

/**********************************************************************/

int main(void)
{
    /* START INITIALIZATIONS */
    // Window Init
    if (!glfwInit())
        return -1;
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Aviso & Baniqued", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // Glad INIT
    gladLoadGL();
    // Input INIT
    glfwSetKeyCallback(window, keyCallback);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetCursorPosCallback(window, cursorCallback);
    // Shader Init
    GLuint shaderProg = compShaderProg("Shader/sample.vert", "Shader/sample.frag");
    glEnable(GL_DEPTH_TEST); 
    /* END INITIALIZATIONS*/

    /* START OBJECT DECLARATIONS */

    /* Point Light */
    // Sphere.obj from: https://thangs.com/designer/GeorgeDebarr/3d-model/sphere.obj-217158
    PointLight* pointLight = new PointLight({ -15.f, 3.f, -5.f });
    Model* lightModel = new Model("3D/Sphere.obj", lightColors[lightColIndex]);
    pointLight->setBrightness(150.f);
    lightModel->setScale(0.01f);
    lightModel->setPosition({-0.5f, 0.5f, 0.f});

    /* Direction Light */
    DirectionLight* directionLight = new DirectionLight(
        glm::vec3(0.f) - glm::vec3( 4.f, - 5.f, 0.f ) // Light Direction calculation for a light at {4, -5, 0} pointing towards center
    ); 
    directionLight->setBrightness(2.f);

    /* Perspective Cam */
    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
    perspectiveCamera->setPosition({0.f, 0.f, -1.f});
    
    /* Orthographic Cam */
    OrthoCamera* orthoCamera = new OrthoCamera();
    orthoCamera->setWorldUp({0.0f, 0.f, 1.f});  // For Controls to be aligned properly
    orthoCamera->setPosition({0.f, 1.f, 0.f});  // Top-down view
    
    /* Main Model */
    // Aircraft.obj and Aircraft.jpg from: https://free3d.com/3d-model/e-45-aircraft-71823.html
    Model* mainModel = new Model("3D/Aircraft.obj", "3D/Aircraft.jpg");
    mainModel->setScale(0.20f);
    
    /* Currently Active Camera */
    Camera* currentCamera = perspectiveCamera;

    /* END OBJECT DECLARATIONS */

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Application & Rendering of Objects */
        pointLight->apply(shaderProg);
        directionLight->apply(shaderProg);
        currentCamera->apply(shaderProg);
        mainModel->render(shaderProg);
        lightModel->render(shaderProg);
        
        /* Render Frame and Wait for inputs, then processes inputs and resets window & depth buffer */
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

/* For keyboard input handling */
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

/* For mouse cursor input */
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

/* Flag resolution */
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
        *cam = perspectiveCam;
        inOrthoView = false;
        onePressed = false;
    }
    if (twoPressed) {
        *cam = orthoCam;
        inOrthoView = true;
        twoPressed = false;
    }
    if (spacePressed) {
        if (controllingLight) {
            controllingLight = false;
        }   
        else {
            controllingLight = true;
            lightColIndex = (lightColIndex + 1) % 7;
            pointLight->setColor(lightColors[lightColIndex]);
            light->setBaseColor(lightColors[lightColIndex]);
        }
           

        spacePressed = false;
    }

    // Disables camera movement while in orthographic
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
        (*cam)->rotateAround(-camPanSpeed, { 0.f, 1.f, 0.f });
        mouseRight = false;
    }
    if (mouseLeft) {
        (*cam)->rotateAround(camPanSpeed, { 0.f, 1.f, 0.f });
        mouseLeft = false;
    }
}