#include <stdio.h>
#include <iostream>
#include <chrono>

#include <core/startup_config.hpp>
#include <core/frame_timer.hpp>
#include <core/frame_limiter.hpp>

#include <renderer/renderer.hpp>
#include <asset/basic_mesh_reader.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader_program.h>
#include <shader.h>
#include <vertex_buffer.h>
#include <index_buffer.h>
#include <file_read_std.h>

#include <shader_program_specializations.h>

using namespace ogllib;

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height);

// put all OpenGL allocations here, so that they're destructed before main() tears down everything else.
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(GLFWwindow *window,
                 core::frame_timer &timer,
                 core::frame_limiter &limiter,
                 ogllib::shader_program<ogllib::vertex_p> &program,
                 ogllib::vertex_array<ogllib::vertex_p> vao,
                 ogllib::index_buffer &ebo);

void process_input(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf.width(), conf.height());

    if (window == NULL)
        return -1;

    run_game(conf, window);

    glfwTerminate();
    spdlog::shutdown();
    return 0;
}

void run_game(core::startup_config &conf, GLFWwindow *window)
{
    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);

    rendering::renderer renderer(conf);
    renderer.init();

    readers::basic_mesh_reader reader;
    auto mesh = reader.read_mesh_p(std::string("./assets/models/square.json"));

    auto error = mesh.error();
    std::cout << error;

    ogllib::shader<FileReadStd> vert(GL_VERTEX_SHADER);
    ogllib::shader<FileReadStd> frag(GL_FRAGMENT_SHADER);
    vert.from_file("./assets/shaders/simple.vert");
    frag.from_file("./assets/shaders/simple.frag");
    ogllib::shader_program<ogllib::vertex_p> program(&vert, &frag);
    program.compile();

    ogllib::vertex_buffer<ogllib::vertex_p> vbo(mesh->vertices);
    ogllib::vertex_array<ogllib::vertex_p> vao;
    ogllib::index_buffer ebo(mesh->indices);

    vao.bind();

    vbo.bind();
    vbo.buffer();

    ebo.bind();
    ebo.buffer();

    program.bind();
    program.set_attrib_pointers();

    vbo.unbind();
    ebo.unbind();
    vao.unbind();

    render_loop(window, timer, limiter, program, vao, ebo);
}

void render_loop(GLFWwindow *window,
                 core::frame_timer &timer,
                 core::frame_limiter &limiter,
                 ogllib::shader_program<ogllib::vertex_p> &program,
                 ogllib::vertex_array<ogllib::vertex_p> vao,
                 ogllib::index_buffer &ebo)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        timer.start();

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        program.bind();
        vao.bind();
        ebo.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        vao.unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        limiter.wait_remainder();

        timer.end();
        //std::cout << timer.frame_info() << std::endl;
    }
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    (void)(window); // suppress unused param
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}