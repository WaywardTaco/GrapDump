
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

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "PerspectiveCamera.hpp"
#include "OrthoCamera.hpp"
#include "Skybox.hpp"
#include "LightSource.hpp"
#include "PointLight.hpp"
#include "DirectionLight.hpp"
#include "Player.hpp"

float
    mouseY = 0.f,
    mouseX = 0.f,
    modelMoveSpd = 0.05f,
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

OrthoCamera* orthoCam = NULL;
Player* player = NULL;

bool usingOrtho = false;

const float
PLAYER_TURN_SPEED = 0.2f,
PLAYER_MOVE_SPEED = 10.f,
ORTHO_CAM_MOVE_SPEED = 10.f;

/**********************************************************************/

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xPos, double yPos);

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,   // Source Factor (Foreground Layer)
        GL_ONE_MINUS_SRC_ALPHA); // Destination Factor (Background Layer)
    // Final Color = ForegroundColor * SourceFactor + BackgroundColor * DestinationFactor
    // glBlendEquation(GL_FUNC_SUBTRACT); changes addition to subtraction
    // See Website for Reference: Anders Riggelsen - Visual glBlendFunc and glBlendEquation Tool

    /* Light declaration */
    PointLight* pointLight = new PointLight({ 0.f, 3.f, -5.f });
    pointLight->setBrightness(10.f);

    PerspectiveCamera* perspectiveCamera = new PerspectiveCamera();
    OrthoCamera* orthoCam = new OrthoCamera();
    orthoCam->setPosition({0.f, 0.f, 1.f});
    
    // TODO: Make 6 different textured models (no normal maps needed) and place randomly
    Model* mainModel = new Model("3D/djSword.obj", "3D/brickwall.jpg");
    Model* yae = new Model("3D/djSword.obj", "3D/brickwall.jpg", "3D/brickwall_normal.jpg");

    yae->setScale(0.02f);
    yae->setPosition({ 0.f, -0.5f, 0.f });
    yae->rotate(90.f, { 0.f, 0.f, 1.f });
    yae->rotate(180.f, {0.f, 1.f, 0.f});

    mainModel->setScale(10.f);
    mainModel->setPosition({ 0.f, 0.5f, 0.f });
    mainModel->rotate(90.f, { 0.f, 0.f, 1.f });
    mainModel->rotate(180.f, { 0.f, 1.f, 0.f });

    // TODO: Make Skybox an Ocean w/ Textures
    Skybox* sky = new Skybox(
        "Skybox/rainbow_rt.png",
        "Skybox/rainbow_lf.png",
        "Skybox/rainbow_up.png",
        "Skybox/rainbow_dn.png",
        "Skybox/rainbow_ft.png",
        "Skybox/rainbow_bk.png");

    // Front-back texture fixing
    glEnable(GL_DEPTH_TEST); 

    Shader* mainShader = new Shader("Shader/sample.vert", "Shader/sample.frag");
    Shader* skyboxShader = new Shader("Shader/Skybox.vert", "Shader/Skybox.frag");
    
    perspectiveCamera->setPosition({0.f, 0.f, -1.f});
    perspectiveCamera->setCenter({0.f, 0.f, 0.f});
    mainModel->setScale(0.01f);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        perspectiveCamera->apply(mainShader, skyboxShader);

        /* Lighting */
        pointLight->apply(mainShader);
        
        sky->render(skyboxShader);
        mainModel->render(mainShader);
        yae->render(mainShader);

        mainModel->rotate(-0.05f, { 0.f, 1.f, 0.f });
        yae->rotate(-0.05f, {0.f, 1.f, 0.f});


        /* Render Frame and Wait for inputs, then resets window & depth buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        //processInput(perspectiveCamera, mainModel, lightModel, pointLight, directionLight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

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

    if (key == GLFW_KEY_W && player != NULL)
        player->move({ 0.f, 0.f, PLAYER_MOVE_SPEED });
    if (key == GLFW_KEY_S && player != NULL)
        player->move({ 0.f, 0.f, -PLAYER_MOVE_SPEED });
    if (key == GLFW_KEY_A && player != NULL)
        player->turn(-PLAYER_TURN_SPEED, { 0.f, 1.f, 0.f });
    if (key == GLFW_KEY_D && player != NULL)
        player->turn(PLAYER_TURN_SPEED, { 0.f, 1.f, 0.f });
    if (key == GLFW_KEY_E && player != NULL)
        player->move({0.f, -PLAYER_MOVE_SPEED, 0.f});
    if (key == GLFW_KEY_Q && player != NULL)
        player->move({0.f, PLAYER_MOVE_SPEED, 0.f});
       
    if (key == GLFW_KEY_SPACE && player != NULL)
        // Shoot Torpedo which lasts 3 sec every 5 sec

    if (key == GLFW_KEY_UP && orthoCam != NULL)
        orthoCam->moveBy({ 0.f, 0.f, ORTHO_CAM_MOVE_SPEED });
    if (key == GLFW_KEY_DOWN && orthoCam != NULL)
        orthoCam->moveBy({ 0.f, 0.f, -ORTHO_CAM_MOVE_SPEED });
    if (key == GLFW_KEY_RIGHT && orthoCam != NULL)
        orthoCam->moveBy({ ORTHO_CAM_MOVE_SPEED, 0.f, 0.f });
    if (key == GLFW_KEY_LEFT && orthoCam != NULL)
        orthoCam->moveBy({ -ORTHO_CAM_MOVE_SPEED, 0.f, 0.f });

    if (key == GLFW_KEY_1 && player != NULL && !usingOrtho)
        player->toggleCamera();
    if (key == GLFW_KEY_2)
        usingOrtho = !usingOrtho;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {

    if (ypos > mouseY)
        //
    if (ypos < mouseY)
        //
    if (xpos > mouseX)
        //
    if (xpos < mouseX)
        //

    mouseX = xpos;
    mouseY = ypos;
}

/*
void processInput(Camera* cam, Model* mainModel, Model* light, PointLight* pointLight, DirectionLight* dirLight) {
    if (wPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 1.f, 0.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 1.f, 0.f, 0.f });
        }
        else
            mainModel->move({ 0.f, modelMoveSpd, 0.f });
        wPressed = false;
    }
    if (sPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 1.f, 0.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 1.f, 0.f, 0.f });
        }
        else
            mainModel->move({ 0.f, -modelMoveSpd, 0.f });
        sPressed = false;
    }
    if (dPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 1.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 1.f, 0.f });
        }
        else
            mainModel->move({ -modelMoveSpd, 0.f, 0.f });
        dPressed = false;
    }
    if (aPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 1.f, 0.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 1.f, 0.f });
        }
        else
            mainModel->move({ modelMoveSpd, 0.f, 0.f });
        aPressed = false;
    }
    if (ePressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 0.f, 1.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, lightRotSpd, { 0.f, 0.f, 1.f });
        }
        else
            mainModel->move({ 0.f, 0.f, modelMoveSpd});
        ePressed = false;
    }
    if (qPressed) {
        if (controllingLight) {
            pointLight->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 0.f, 1.f });
            light->rotateAround({ 0.f, 0.f, 0.f }, -lightRotSpd, { 0.f, 0.f, 1.f });
        }
        else
            mainModel->move({0.f, 0.f, -modelMoveSpd});
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
        onePressed = false;
    }
    if (twoPressed) {
        // Cam is set to ortho
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
    if (mouseUp) {
        cam->rotateAround(camPanSpeed, {1.f, 0.f, 0.f});
        mouseUp = false;
    }
    if (mouseDown) {
        cam->rotateAround(-camPanSpeed, { 1.f, 0.f, 0.f });
        mouseDown = false;
    }
    if (mouseRight) {
        cam->rotateAround(camPanSpeed, { 0.f, 1.f, 0.f });
        mouseRight = false;
    }
    if (mouseLeft) {
        cam->rotateAround(-camPanSpeed, { 0.f, 1.f, 0.f });
        mouseLeft = false;
    }
}
*/

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