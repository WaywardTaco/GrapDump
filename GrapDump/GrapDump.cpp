
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

float
    x_mod = 0,
    y_mod = 1,
    z_mod = 0,
    a_mod = 0
;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawPentagon(float centerX, float centerY, float radius, float phaseDegrees);

int main(void)
{
    // Load the vertex shader file into a string
    std::fstream vertSrc("Shader/sample.vert");
    std::stringstream vertBuff;

    // Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    // Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    // Load the fragment shader file into a character array
    std::fstream fragSrc("Shader/sample.frag");
    std::stringstream fragBuff;
    fragBuff << fragSrc.rdbuf();
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    // Declare window
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Josiah Aviso", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initializations */
    gladLoadGL();

    // Assign Key Callback Function to the current window
    glfwSetKeyCallback(window, keyCallback);

    // Shaders compile at runtime

    // Start with an unsigned int to compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Assign the source to the vertex shader
    glShaderSource(
        vertexShader,   // The shader
        1,              // Number to compile
        &v,             // source for compilation
        NULL            // ???
    );
    // Compile the vertex shader
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);

    // Create a Shader Program to combine the Shaders together as a PAIRED operation (must be PAIRED frag & vert)
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader); // Attach compiled vertex shader
    glAttachShader(shaderProg, fragmentShader); // Attach compiled fragment shader

    // Finalize shader compilation
    glLinkProgram(shaderProg); // This is what loads the shader into the program

    // Load the Model
    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    // Note: the drawing of this is done per model
    GLfloat vertices[]{
        // x, y, z
        0.f, 0.5f, 0.f,   // Vertex 0
        -0.5f, -0.5f, 0.f,    // Vertex 1
        0.5f, -0.5f, 0.f,    // Vertex 2
    };
    
    GLuint indices[] = {
        0, 1, 2, // Triangle A Vertex Indecies
    };

    // Initialize VAO & VBO & EBO, generates their values
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Currently editing VAO = null
    glBindVertexArray(VAO);
    // Currently editing VAO = VAO variable

    // Currently editing VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // VAO variable <- VBO variable *not null <- VBO variable
    // Currently editing VBO = VBO variable

    glBufferData(
        GL_ARRAY_BUFFER,    // Type of data in the buffer
        sizeof(GL_FLOAT) * attributes.vertices.size(),   // Size of the array
        attributes.vertices.data(), // Array of points, feeds the whole array *Note: can also do &attributes.vertices[0]
        GL_STATIC_DRAW      // For optimizing the render
    );

    glVertexAttribPointer(
        0,                  // Index, also type of data, 0 is positional data, 1 is textures?, etc...
        3,                  // Number of components (x, y, z)
        GL_FLOAT,           // Data type of compontents
        GL_FALSE,           // If data is normalized or not
        3 * sizeof(float),  // Byte size of each vertex
        (void*)0            // ???
    );
    
    // Binds currently editing buffer to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * mesh_indices.size(),
        mesh_indices.data(),
        GL_STATIC_DRAW
    );
    
    glEnableVertexAttribArray(0); // Index 0 used since prev buffer is declared to index 0

    // currently editing VBO = VBO variable
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // currently editing VBO = null

    // currently editing VAO = VAO variable
    glBindVertexArray(0);
    // currently editing VAO = null

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Reset so the model isnt accidentally edited


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Resets the window to clear to prevent artifacts
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Transfer Code

        // Sample Code
        // unsigned int xLoc = glGetUniformLocation(shaderProg, "x"); // Grabs variable named 'x' from the shader
        // glUniform1f(xLoc, x_mod); // Assigns one float from x_mod to xLoc which refers to x in the shader
        // unsigned int yLoc = glGetUniformLocation(shaderProg, "y");
        // glUniform1f(yLoc, y_mod);

        // Initialize identity matrix
        glm::mat4 identityMat4 = glm::mat4(1.f);

        // Apply transforms to a matrix

        // Translate
        glm::mat4 transformMat = glm::translate(
            identityMat4,
            glm::vec3(z_mod, a_mod, 0.f)
        );

        // Scale
        transformMat = glm::scale(
            transformMat,
            glm::vec3(y_mod, y_mod, y_mod)
        );

        // Rotate
        transformMat = glm::rotate(
            transformMat,
            glm::radians(x_mod),
            glm::normalize(
                glm::vec3(0.f, 1.f, 0.f)
            )
        );

        // Get the transform matrix in the vertex shader and assign the transformMat here
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(                 // Specifies a 4x4 matrix with floats as vectors
            transformLoc,                   // Target to copy to
            1,                              // Number of matricies to copy
            GL_FALSE,                       // If will Transpose
            glm::value_ptr(transformMat)    // Transform matrix pointer
        );

        // DRAW Code for Model 1
        
        glUseProgram(shaderProg); // Applies shaders to all follwoing vertecies

        glBindVertexArray(VAO); // What array is drawn

        /*
        glDrawArrays(
            GL_TRIANGLES,       // Type of primitives drawn
            0,                  // Start from index 0
            3                   // Number of components
        );   
        */

        glDrawElements(
            GL_TRIANGLES,       // Type of primitives drawn
            mesh_indices.size(),    // Size of array
            GL_UNSIGNED_INT,    // Type of array
            0                   // Start at index 0
        );

        // DRAW Code for Model 2...

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Garbage collection, clears all the buffer objects & vertex array objects
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void drawPentagon(float centerX, float centerY, float radius, float phaseDegrees) {
   
    float phaseRadians = phaseDegrees * M_PI / 180;
    
    glBegin(GL_POLYGON);
    // Window coords are -1.0 to 1.0
    // Order matters *sequential
   
    for (int i = 0; i < 5; i++) {
        float 
            angle = phaseRadians + i * 2 * M_PI / 5 ;

        float 
            pointX = radius * cos(angle) + centerX,
            pointY = radius * sin(angle) + centerY;
        
        glVertex2f(pointX, pointY);
    }

    glEnd();
}

void keyCallback(
    GLFWwindow* window,     // Pointer to the window being checked
    int key,                // the keycode being pressed
    int scancode,           // Physical position of the press on keyboard
    int action,             // Either pressed or released
    int mods                // Which modifier keys are held (like Shift, Ctrl, Alt, etc.)
) 
{
  
    if (key == GLFW_KEY_D && mods == GLFW_MOD_SHIFT)
        x_mod += 15.f;
    if (key == GLFW_KEY_A && mods == GLFW_MOD_SHIFT)
        x_mod -= 15.f;
    if (key == GLFW_KEY_W && mods == GLFW_MOD_SHIFT)
        y_mod += 0.1f;
    if (key == GLFW_KEY_S && mods == GLFW_MOD_SHIFT)
        y_mod -= 0.1f;

    if (key == GLFW_KEY_D && mods != GLFW_MOD_SHIFT)
        z_mod += 0.05f;
    if (key == GLFW_KEY_A && mods != GLFW_MOD_SHIFT)
        z_mod -= 0.05f;
    if (key == GLFW_KEY_W && mods != GLFW_MOD_SHIFT)
        a_mod += 0.05f;
    if (key == GLFW_KEY_S && mods != GLFW_MOD_SHIFT)
        a_mod -= 0.05f;
}