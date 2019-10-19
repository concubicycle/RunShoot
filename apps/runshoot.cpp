//
// Created by sava on 10/18/19.
//

/////////////
#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

using namespace gl;
using namespace glbinding;
using GLenum = gl::GLenum;
//////////////

#include "runshoot.hpp"



GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height, core::startup_config& config)
{
    GLFWwindow *window;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    if (config.fullscreen())
    {
        const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window = glfwCreateWindow(mode->width, mode->height, "RunShoot", glfwGetPrimaryMonitor(), NULL);
    } else
    {
        window = glfwCreateWindow(width, height, "RunShoot", nullptr, nullptr);
    }

    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress, true);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);

    return window;
}