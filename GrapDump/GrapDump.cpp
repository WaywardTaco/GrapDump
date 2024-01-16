#define _USE_MATH_DEFINES

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

void drawPentagon(float centerX, float centerY, float radius, float phaseDegrees);

int main(void)
{
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

    float 
        phaseDegrees = 90.0,
        radius = 0.5
    //    radiusInc = 0.0001
    ;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // DRAW Code
        drawPentagon(0.0, 0.0, radius, phaseDegrees);

        /*
        phaseDegrees = phaseDegrees - 0.01;

        if (radius >= 1.0 || radius <= 0.1)
            radiusInc = -radiusInc;

        radius += radiusInc;
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

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

