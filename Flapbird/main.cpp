#include <iostream>
#include "glm/glm.hpp"
#include <vector>
#include <GLFW/glfw3.h>
#include "models.h"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


using namespace std;



 Models cubo = Models();


//void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );




int main()
{



    GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "FlapBird 3D", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        /* Make the window's context current */
            glfwMakeContextCurrent(window);

            /* Loop until the user closes the window */
            while (!glfwWindowShouldClose(window))
            {
                /* Render here */
                glClear(GL_COLOR_BUFFER_BIT);
               cubo.drawCube();


                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
            }

            glfwTerminate();
    return 0;
}
