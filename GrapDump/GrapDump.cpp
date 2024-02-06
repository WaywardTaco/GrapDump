
#define _USE_MATH_DEFINES
#define TINYOBJLOADER_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <string>
#include <iostream>

#include "tiny_obj_loader.h"

float
    rotation = 0.f,
    offset = 0.5f
;

float
    window_height = 600,
    window_width = 600
;


GLuint genShaderProg(std::string vertShader, std::string fragShader);

int main(void)
{
    /* Initializations */
    if (!glfwInit())
        return -1;

    GLFWwindow* window 
        = glfwCreateWindow(window_width, window_height, "Josiah Kurt B. Aviso", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    GLuint shaderProg = genShaderProg("Shader/sample.vert", "Shader/sample.frag");

    /* Load Model */
    std::string                         path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t>       shapes;
    std::vector<tinyobj::material_t>    material;
    std::string                         warning, error;
    tinyobj::attrib_t                   attributes;
    std::vector<GLuint>                 mesh_indices;
    bool success 
        = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);

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

    /* Clean up editing buffers/arrays */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Reset at start of frame */
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4   transformMat1 = glm::mat4(1.f),
                    transformMat2 = glm::mat4(1.f),
                    transformMat3 = glm::mat4(1.f);

        /* Model 1 */
        transformMat1 = glm::rotate(transformMat1,
            glm::radians(rotation),
            glm::normalize(glm::vec3(0.f, 0.f, 1.f)));

        transformMat1 = glm::translate(transformMat1,
            glm::vec3(offset, 0.f, 0.f));

        unsigned int transformLoc1 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, glm::value_ptr(transformMat1));

        glUseProgram(shaderProg); 
        glBindVertexArray(VAO); 
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);
        
        /* Model 2 */
        transformMat2 = glm::rotate(transformMat2,
            glm::radians(rotation + 120.f),
            glm::normalize(glm::vec3(0.f, 0.f, 1.f)));

        transformMat2 = glm::translate(transformMat2,
            glm::vec3(offset, 0.f, 0.f));

        unsigned int transformLoc2 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(transformMat2));

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Model 3 */
        transformMat3 = glm::rotate(transformMat3,
            glm::radians(rotation + 240.f),
            glm::normalize(glm::vec3(0.f, 0.f, 1.f)));

        transformMat3 = glm::translate(transformMat3,
            glm::vec3(offset, 0.f, 0.f));

        unsigned int transformLoc3 = glGetUniformLocation(shaderProg, "transform");
        glUniformMatrix4fv(transformLoc3, 1, GL_FALSE, glm::value_ptr(transformMat3));

        glUseProgram(shaderProg);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* End of frame processing */
        glfwSwapBuffers(window);
        glfwPollEvents();

        rotation += 0.01f;
    }

    /* Cleaning */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

GLuint genShaderProg(std::string vertShader, std::string fragShader) {

    std::fstream        
        vertSrc(vertShader),
        fragSrc(fragShader);
    std::stringstream   
        vertBuff, fragBuff;
    GLuint
        vertexShader, fragmentShader, shaderProg;

                        vertBuff << vertSrc.rdbuf();
    std::string         vertS = vertBuff.str();
    const char*         v = vertS.c_str();

                        fragBuff << fragSrc.rdbuf();
    std::string         fragS = fragBuff.str();
    const char*         f = fragS.c_str();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &v, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &f, NULL);
    glCompileShader(fragmentShader);

    shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    return shaderProg;
};