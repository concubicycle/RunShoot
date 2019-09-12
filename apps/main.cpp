#include <stdio.h>
#include <iostream>

#include <GLFW/glfw3.h>

#include "core/startup_config.hpp"
#include "core/frame_timer.hpp"
#include "core/frame_limiter.hpp"

#include <chrono>

GLFWwindow *make_window(std::uint32_t width, std::uint32_t height);

int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);

    auto window = make_window(conf.width(), conf.height());

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

        limiter.wait_remainder();

        timer.end();
        std::cout << timer.frame_info() << std::endl;
    }

    glfwTerminate();

    return 0;
}

GLFWwindow *make_window(std::uint32_t width, std::uint32_t height)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "RunShoot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);
    return window;
}