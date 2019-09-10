#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.h>

#include "core/startup_config.hpp"
#include "core/frame_timer.hpp"

#include <chrono>

int main(int argc, char **argv)
{
    core::startup_config conf = core::startup_config();
    core::frame_timer timer;
    conf.load();

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(conf.width(), conf.height(), "RunShoot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        timer.start();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        timer.end();

        auto ns = std::chrono::duration_cast<std::chrono::milliseconds>(timer.delta());
        auto ns_smooth = std::chrono::duration_cast<std::chrono::milliseconds>(timer.smoothed_delta());

        std::cout << "Delta: " << ns.count() << std::endl;
        std::cout << "Smoothed delta: " << ns_smooth.count() << std::endl
                  << std::endl;
    }

    glfwTerminate();

    return 0;
}