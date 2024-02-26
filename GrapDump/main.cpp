
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <chrono>
#include <string>
#include <iostream>

#include "Model.hpp"
#include "Camera.hpp"

float
    window_height = 640,
    window_width = 640;

float
    moveSpd = 0.2f,
    rotSpd = 0.5f;

float
    cooldown = 0.f,
    timelimit = 3000.f;

float
mouseX = 0.f,
mouseY = 0.f;

auto 
    start = std::chrono::steady_clock::now(),
    timeElapsed = std::chrono::steady_clock::now();

Camera* camera = new Camera(window_height / window_width);
std::vector<Model*> models;

GLFWwindow* initGLFW();

GLuint compShaderProg(std::string vertShaderSrc, std::string fragShaderSrc);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv);

int main(void)
{
    GLFWwindow* window = initGLFW();
    if (window == NULL)
        return -1;

    models.push_back(new Model("3D/Dragon.obj"));

    GLuint shaderProg = compShaderProg("Shader/sample.vert", "Shader/sample.frag");



    while (!glfwWindowShouldClose(window))
    {
        setShaderMat4fv(shaderProg, "view", camera->getView());
        setShaderMat4fv(shaderProg, "projection", camera->getProjection());

        for (int i = 0; i < models.size(); i++)
            models[i]->render(shaderProg);

        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       

        if (cooldown > 0.f) {
            timeElapsed = std::chrono::steady_clock::now();
            cooldown = timelimit - (float) std::chrono::duration_cast<std::chrono::milliseconds>(timeElapsed - start).count();
            std::cout << cooldown << std::endl;
        }
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* initGLFW() {
    if (!glfwInit())
        return NULL;

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Josiah Aviso", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetKeyCallback(window, keyCallback);
    
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorCallback);

    return window;
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

    if (key == GLFW_KEY_SPACE && cooldown <= 0.f) {
        Model* temp = new Model("3D/Dragon.obj");
        temp->setPosition(camera->center);

        models.push_back(temp);

        cooldown = timelimit;
        start = std::chrono::steady_clock::now();
    }

    if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
        camera->move({ 0.f, 0.f, -moveSpd });
        camera->pan({ 0.f, 0.f, -moveSpd });
    }
    if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
        camera->move({ -moveSpd, 0.f, 0.f });
        camera->pan({ -moveSpd, 0.f, 0.f });
    }
    if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
        camera->move({ 0.f, 0.f, moveSpd });
        camera->pan({ 0.f, 0.f, moveSpd });
    }
    if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
        camera->move({ moveSpd, 0.f, 0.f });
        camera->pan({ moveSpd, 0.f, 0.f });
    }
    if (key == GLFW_KEY_LEFT_ALT && action != GLFW_RELEASE) {
        camera->move({ 0.f, moveSpd, 0.f });
        camera->pan({ 0.f, moveSpd, 0.f });
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action != GLFW_RELEASE) {
        camera->move({ 0.f, -moveSpd, 0.f });
        camera->pan({ 0.f, -moveSpd, 0.f });
    }

    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
        camera->pan({ 0.f, rotSpd, 0.f});
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
        camera->pan({ 0.f, -rotSpd, 0.f });
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
        camera->pan({ -rotSpd, 0.f, 0.f });
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
        camera->pan({ rotSpd, 0.f, 0.f });
};

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    if (ypos > mouseY)
        camera->pan({ 0.f, rotSpd, 0.f });
    if (ypos < mouseY)
        camera->pan({ 0.f, -rotSpd, 0.f });
    if (xpos > mouseX)
        camera->pan({ rotSpd, 0.f, 0.f });
    if (xpos < mouseX)
        camera->pan({ -rotSpd, 0.f, 0.f });

    mouseX = xpos;
    mouseY = ypos;
}

void setShaderMat4fv(GLuint shaderProg, const GLchar* variable, glm::mat4 matrix4fv) {
    unsigned int varLoc = glGetUniformLocation(shaderProg, variable);
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(matrix4fv));
};
