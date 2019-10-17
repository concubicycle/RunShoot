#include <iostream>

/////////////
#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

using namespace gl;
using namespace glbinding;
using GLenum = gl::GLenum;
//////////////

#include <core/startup_config.hpp>
#include <core/frame_timer.hpp>
#include <core/frame_limiter.hpp>
#include <core/input_manager.hpp>
#include <renderer/renderer.hpp>
#include <asset/basic_mesh_reader.hpp>
#include <file_read_std.h>
#include <asset/texture_manager.hpp>
#include <ecs/entity_factory.hpp>
#include <ecs/entity_world.hpp>
#include <asset/scene_loader.hpp>

struct scene_data
{
    GLFWwindow *window;
    core::frame_timer &timer;
    core::frame_limiter &limiter;
    core::input_manager& input;
    rendering::renderer& renderer;
    asset::scene& scene;

};

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height);

// put all OpenGL allocations here, so that they're destructed before main() tears down everything else.
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(scene_data &data);



int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf.width(), conf.height());

    if (window == nullptr)
        return -1;

    run_game(conf, window);

    glfwTerminate();
    spdlog::shutdown();
    return 0;
}

void run_game(core::startup_config &conf, GLFWwindow *window)
{
    ecs::entity_factory factory(1);
    ecs::entity_world entities(factory);
    asset::scene_loader loader;

    auto scene = loader.load_scene("./assets/scenes/scene.json", entities);

    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);
    core::input_manager input(window);

    asset::texture_manager textures;

    rendering::renderer renderer(conf, textures);
    renderer.init();

    readers::basic_mesh_reader reader;

    entities.for_all_entities([&reader, &renderer] (ecs::entity& e) {
        auto &r = e.get_component<ecs::render_component>();
        switch (r.mesh_format)
        {
            case ecs::mesh_type::P_TX2D:
                auto mesh = reader.read_mesh_ptx2d(r.mesh_path);
                renderer.init_render_component(r, *mesh);
                break;
        }
    });

    scene_data data = {window, timer, limiter, input, renderer, scene };
    render_loop(data);
}

void render_loop(scene_data &data)
{
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();
        gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        gl::glClear(gl::ClearBufferMask::GL_COLOR_BUFFER_BIT);
        data.renderer.draw_scene(data.scene);

        /* Swap front and back buffers */
        glfwSwapBuffers(data.window);

        /* Poll for and process events */
        glfwPollEvents();

        /* Wait until end of frame */
        data.limiter.wait_remainder();

        data.timer.end();
    }
}

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "RunShoot", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glbinding::initialize(glfwGetProcAddress, true);

    /* Prevent framerate cap by gpu driver (don't wait for vblank before returning form glfwSwapBuffers) */
    glfwSwapInterval(0);

    return window;
}
