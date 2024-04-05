
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
#include "SpotLight.hpp"
#include "Player.hpp"

float
mousePrevY = 0.f,
mousePrevX = 0.f;

float skyIntensities[] = {
    2.f,
    1.f,
    0.2f
};
int skyIntensity = 0;

const float
    window_height = 1000,
    window_width = 1000;
const char* 
    window_name = "Aviso & Baniqued";

GLFWgamepadstate lastState;

DirectionLight* skylight = NULL;
OrthoCamera* orthoCam = NULL;
Player* player = NULL;

bool 
usingOrtho = false,
lMousePressed = false;

const float
PLAYER_TURN_SPEED = 1.f,
PLAYER_MOVE_SPEED = 0.01f,
ORTHO_CAM_MOVE_SPEED = 0.02f,
ORTHO_CAM_PAN_SPEED = 1.f,
THIRD_POV_CAM_PAN_SPEED = 1.f,
GAMEPAD_INPUT_MOD = 0.08f,
GAMEPAD_JOYSTICK_THRESHOLD = 0.2f;

/**********************************************************************/

/**********************************************************************
*   Controller Bindings:
*       Ascend/Descend - A/B Buttons / X/Circle Buttons
*       Forward/Backward - Up/Down Dpad
*       Turn Left/Right - Left/Right Dpad
*       Switch between first-person & third-person - Left Bumper / L1
*       Rotate third-person camera - Right Stick (Left/Right)
*       Switch to/from orthographic camera - Right Bumper / R1
*       Move orthographic camera - Left Stick while in ortho
*       Rotate orthographic camera - Right Stick while in ortho
*       Cycle through skylight brightness - Y Button / Triangle Button
***********************************************************************/

void processGamepad();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
GLFWwindow* initializeGLFW();

int main(void)
{
    GLFWwindow* window = initializeGLFW();
    if (window == NULL)
        return -1;

    Shader* mainShader = new Shader("Shader/sample.vert", "Shader/sample.frag");
    Shader* skyboxShader = new Shader("Shader/Skybox.vert", "Shader/Skybox.frag");
    Shader* monochromeShader = new Shader("Shader/sample.vert", "Shader/monochrome.frag");
    Shader* monoSkyShader = new Shader("Shader/Skybox.vert", "Shader/monochromeSky.frag");

    Shader* activeModelShader = NULL;
    Shader* activeSkyShader = NULL;

    // See Website for Reference: Anders Riggelsen - Visual glBlendFunc and glBlendEquation Tool
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD); 

    /* Object Declarations */
    orthoCam = new OrthoCamera();

    // Source: https://jkhub.org/files/file/3216-underwater-skybox/
    Skybox* sky = new Skybox(
        "Skybox/uw_rt.png",
        "Skybox/uw_lf.png",
        "Skybox/uw_up.png",
        "Skybox/uw_dn.png",
        "Skybox/uw_ft.png",
        "Skybox/uw_bk.png");

    skylight = new DirectionLight();
    skylight->setBrightness(skyIntensities[skyIntensity]);

    // TODO: Make 6 different textured models (no normal maps needed) and place randomly
    std::vector<Model*> models = {
        new Model("3D/goose.obj", "3D/goose.jpg"),
        new Model("3D/dolphin.obj", "3D/dolphin.jpg"),
        new Model("3D/frog.obj", "3D/frog.jpg"),
        new Model("3D/bluefish.obj", "3D/bluefish.jpg"),
        new Model("3D/angler.obj", "3D/angler.jpg"),
        new Model("3D/alligator.obj", "3D/alligator.jpg")
    };

    Model* ship = new Model("3D/fish.obj", "3D/fish.jpg");
    ship->setScale(0.02f);

    player = new Player(
        ship, 
        new SpotLight(), 
        new PerspectiveCamera(),
        new PerspectiveCamera());

    models[0]->setPosition({0.6f, -0.2f, 2.8f});
    models[0]->setScale(0.5f);

    models[1]->setPosition({ 1.5f, -0.5f, 0.0f });
    models[1]->setScale(0.1f);

    models[2]->rotate(180.0f, { 0.f, 1.f, 0.f });
    models[2]->setPosition({ -2.0f, -0.2f, 0.7f });
    models[2]->setScale(0.02f);

    models[3]->setPosition({ 0.0f, -0.75f, -1.5f });
    models[3]->setScale(0.001f);

    models[4]->setPosition({ 1.25f, -1.0f, 1.70f });
    models[4]->setScale(0.01f);

    models[5]->rotate(270.0f, { 1.f, 0.f, 0.f });
    models[5]->setPosition({ -1.0f, -1.50f, 0.0f });
    models[5]->setScale(0.001f);
    
    orthoCam->setPosition({0.f, 1.f, 0.f});
    orthoCam->setWorldUp({0.f, 0.f, 1.f});
    orthoCam->setZnear(0.01f);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        activeModelShader = mainShader;
        activeSkyShader = skyboxShader;

        if (player->isUsingFirstPOV() && !usingOrtho) {
            activeModelShader = monochromeShader;
            activeSkyShader = monoSkyShader;
        }

        skylight->apply(activeModelShader);
        player->apply(activeModelShader, activeSkyShader);

        if (usingOrtho) {
            orthoCam->apply(activeModelShader, activeSkyShader);
        }

        sky->render(activeSkyShader);

        for (Model* model : models)
            model->render(activeModelShader);

        player->render(activeModelShader);

        std::cout << "Depth: " << player->getPosition().y << std::endl;

        /* Render Frame and Wait for inputs, then resets window & depth buffer */
        glfwSwapBuffers(window);
        glfwPollEvents();
        processGamepad();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();
    return 0;
}

void processGamepad() {
    GLFWgamepadstate gamepad;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad);

    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP])
        player->move({ -PLAYER_MOVE_SPEED * GAMEPAD_INPUT_MOD , 0.f, 0.f });
    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN])
        player->move({ PLAYER_MOVE_SPEED * GAMEPAD_INPUT_MOD, 0.f, 0.f });
    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT])
        player->turn(PLAYER_TURN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 1.f, 0.f });
    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT])
        player->turn(-PLAYER_TURN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 1.f, 0.f });

    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_A] && player->getPosition().y + PLAYER_MOVE_SPEED < 0.f)
        player->move({ 0.f, 0.f, PLAYER_MOVE_SPEED * GAMEPAD_INPUT_MOD });
    if(gamepad.buttons[GLFW_GAMEPAD_BUTTON_B])
        player->move({ 0.f, 0.f, -PLAYER_MOVE_SPEED * GAMEPAD_INPUT_MOD });

    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && !usingOrtho && !lastState.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER])
        player->toggleCamera();
    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] && !lastState.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER])
        usingOrtho = !usingOrtho;

    if (gamepad.buttons[GLFW_GAMEPAD_BUTTON_Y] && !lastState.buttons[GLFW_GAMEPAD_BUTTON_Y]) {
        skyIntensity = (skyIntensity + 1) % 3;
        skylight->setBrightness(skyIntensities[skyIntensity]);
    }

    if (gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_X] > GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->moveBy({ -ORTHO_CAM_MOVE_SPEED * GAMEPAD_INPUT_MOD, 0.f, 0.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_X] < -GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->moveBy({ ORTHO_CAM_MOVE_SPEED * GAMEPAD_INPUT_MOD, 0.f, 0.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] > GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->moveBy({ 0.f, 0.f, -ORTHO_CAM_MOVE_SPEED * GAMEPAD_INPUT_MOD });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] < -GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->moveBy({ 0.f, 0.f, ORTHO_CAM_MOVE_SPEED * GAMEPAD_INPUT_MOD });
    }

    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->rotateAround(orthoCam->getCenter(), ORTHO_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 0.f, 1.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->rotateAround(orthoCam->getCenter(), -ORTHO_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 0.f, 1.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] > GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->rotateAround(orthoCam->getCenter(), -ORTHO_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 1.f, 0.f, 0.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y] < -GAMEPAD_JOYSTICK_THRESHOLD && usingOrtho) {
        orthoCam->rotateAround(orthoCam->getCenter(), ORTHO_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 1.f, 0.f, 0.f });
    }

    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] > GAMEPAD_JOYSTICK_THRESHOLD && !usingOrtho) {
        player->panCamera(THIRD_POV_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 1.f, 0.f });
    }
    if (gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_X] < -GAMEPAD_JOYSTICK_THRESHOLD && !usingOrtho) {
        player->panCamera(-THIRD_POV_CAM_PAN_SPEED * GAMEPAD_INPUT_MOD, { 0.f, 1.f, 0.f });
    }

    lastState = gamepad;

        /*****************************************************************
*   Controller Bindings
*       Rotate third-person camera - Right Stick (Left/Right)
*       Rotate orthographic camera - Right Stick while in ortho
******************************************************************/
}

void keyCallback(
    GLFWwindow* window,     // Pointer to the window being checked
    int key,                // the keycode being pressed
    int scancode,           // Physical position of the press on keyboard
    int action,             // Either pressed or released
    int mods                // Which modifier keys are held (like Shift, Ctrl, Alt, etc.)
)
{
    if (key == GLFW_KEY_W && player != NULL && action != GLFW_RELEASE)
        player->move({ -PLAYER_MOVE_SPEED, 0.f, 0.f });
    if (key == GLFW_KEY_S && player != NULL && action != GLFW_RELEASE)
        player->move({ PLAYER_MOVE_SPEED, 0.f, 0.f });
    if (key == GLFW_KEY_A && player != NULL && action != GLFW_RELEASE)
        player->turn(PLAYER_TURN_SPEED, { 0.f, 1.f, 0.f });
    if (key == GLFW_KEY_D && player != NULL && action != GLFW_RELEASE)
        player->turn(-PLAYER_TURN_SPEED, { 0.f, 1.f, 0.f });
    if (key == GLFW_KEY_E && player != NULL && action != GLFW_RELEASE)
        player->move({0.f, 0.f, -PLAYER_MOVE_SPEED});
    if (key == GLFW_KEY_Q && player != NULL && player->getPosition().y + PLAYER_MOVE_SPEED < 0.f && action != GLFW_RELEASE)
        player->move({0.f, 0.f, PLAYER_MOVE_SPEED});

    if (key == GLFW_KEY_SPACE && player != NULL && action != GLFW_RELEASE) {
        // Shoot Torpedo which lasts 3 sec every 5 sec

    }

    if (key == GLFW_KEY_UP && orthoCam != NULL && usingOrtho && action != GLFW_RELEASE)
        orthoCam->moveBy({ 0.f, 0.f, ORTHO_CAM_MOVE_SPEED });
    if (key == GLFW_KEY_DOWN && orthoCam != NULL && usingOrtho && action != GLFW_RELEASE)
        orthoCam->moveBy({ 0.f, 0.f, -ORTHO_CAM_MOVE_SPEED });
    if (key == GLFW_KEY_RIGHT && orthoCam != NULL && usingOrtho && action != GLFW_RELEASE)
        orthoCam->moveBy({ -ORTHO_CAM_MOVE_SPEED, 0.f, 0.f });
    if (key == GLFW_KEY_LEFT && orthoCam != NULL && usingOrtho && action != GLFW_RELEASE)
        orthoCam->moveBy({ ORTHO_CAM_MOVE_SPEED, 0.f, 0.f });

    if (key == GLFW_KEY_1 && player != NULL && !usingOrtho && action != GLFW_RELEASE)
        player->toggleCamera();
    if (key == GLFW_KEY_2 && action != GLFW_RELEASE)
        usingOrtho = !usingOrtho;

    if (key == GLFW_KEY_F && action != GLFW_RELEASE) {
        skyIntensity = (skyIntensity + 1) % 3;
        skylight->setBrightness(skyIntensities[skyIntensity]);
    }
}

GLFWwindow* initializeGLFW() {
    if (!glfwInit())
        return NULL;

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, window_name, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetKeyCallback(window, keyCallback);

    glEnable(GL_DEPTH_TEST);
    
    return window;
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    if (!lMousePressed)
        return;

    if (!usingOrtho) {
        if (xpos < mousePrevX)
            player->panCamera(THIRD_POV_CAM_PAN_SPEED, {0.f, 1.f, 0.f});
        if (xpos > mousePrevX)
            player->panCamera(-THIRD_POV_CAM_PAN_SPEED, { 0.f, 1.f, 0.f });
    }

    if (usingOrtho) {
        if (std::abs(xpos - mousePrevX) >= std::abs(ypos - mousePrevY)) {
            if (xpos > mousePrevX)
                orthoCam->rotateAround(orthoCam->getCenter(), ORTHO_CAM_PAN_SPEED, {0.f, 0.f, 1.f});
            if (xpos < mousePrevX)
                orthoCam->rotateAround(orthoCam->getCenter(), -ORTHO_CAM_PAN_SPEED, { 0.f, 0.f, 1.f });
        }
        else {
            if (ypos > mousePrevY)
                orthoCam->rotateAround(orthoCam->getCenter(), ORTHO_CAM_PAN_SPEED, { 1.f, 0.f, 0.f });
            if (ypos < mousePrevY)
                orthoCam->rotateAround(orthoCam->getCenter(), -ORTHO_CAM_PAN_SPEED, { 1.f, 0.f, 0.f });
        }
    }

    mousePrevY = ypos;
    mousePrevX = xpos;
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            lMousePressed = true;
        if (action == GLFW_RELEASE)
            lMousePressed = false;
    }
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