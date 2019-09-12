#include <stdio.h>
#include <iostream>
#include <chrono>

#include "core/startup_config.hpp"
#include "core/frame_timer.hpp"
#include "core/frame_limiter.hpp"

#include "renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height);
bool setup_opengl(std::uint32_t width, std::uint32_t height);
void processInput(GLFWwindow *window);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);

    rendering::renderer renderer(conf);

    auto window = set_up_glfw(conf.width(), conf.height());

    if (window == NULL)
        return -1;

    renderer.init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        timer.start();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        processInput(window);

        limiter.wait_remainder();

        timer.end();
        std::cout << timer.frame_info() << std::endl;
    }

    glfwTerminate();

    return 0;
}

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

bool setup_opengl(std::uint32_t width, std::uint32_t height)
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)(window); // suppress unused param
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}