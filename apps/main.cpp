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
#include <util/debounce.hpp>
#include <chrono>

using float_seconds = std::chrono::duration<float>;

#include "runshoot.hpp"

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
    asset::basic_mesh_reader reader;
    asset::scene_loader loader;
    asset::texture_manager textures;

    rendering::renderer renderer(conf, textures);

    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);
    core::input_manager input(window);

    ecs::entity_factory factory(1);
    ecs::entity_world entities(factory);

    auto scene = loader.load_scene("./assets/scenes/scene.json", entities);

    renderer.init();

    entities.for_all_entities([&reader, &renderer] (ecs::entity& e) {
        auto &r = e.get_component<ecs::render_component>();

        auto ptx2d_load = [&r, &renderer](models::textured_mesh<ogllib::vertex_ptx2d> mesh)
        {
            renderer.init_render_component(r, mesh);
        };

        switch (r.mesh_format)
        {
            case ecs::mesh_type::P_TX2D:
                reader.read_mesh_ptx2d(r.mesh_path).map(ptx2d_load);
                break;
        }
    });

	textures.unload_all();

    game_systems data = { window, timer, limiter, input, renderer, scene };
    render_loop(data);
}

void render_loop(game_systems &data)
{
    auto& entity = data.scene.entity_world().get_entity(123);
    auto angular_v = 3.14f / 3.f;
    auto speed = 5.f;

    debounce print_frametime_debounce(float_seconds(1.f), [&data](){
        std::cout << std::endl << "========" << std::endl << data.timer.frame_info() << std::endl;
    });

    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();
        auto frame_time = data.timer.smoothed_delta_secs();

        // rotate entity. TODO: Implement behaviors somehow
        auto& t = entity.get_component<ecs::transform_component>();
        t.yaw += (angular_v * frame_time);

        if (t.yaw >  6.2831853)
            t.yaw = 0;

        //
        if (data.input.is_key_down(GLFW_KEY_A))
        {
            t.x -= frame_time * speed;
        }
        else if (data.input.is_key_down(GLFW_KEY_D))
        {
            t.x += frame_time * speed;
        }
        if (data.input.is_key_down(GLFW_KEY_W))
        {
            t.y += frame_time * speed;
        }
        else if (data.input.is_key_down(GLFW_KEY_S))
        {
            t.y -= frame_time * speed;
        }

        //
        if (data.input.was_key_pressed(GLFW_KEY_A))
        {
            t.scale_x *= 0.5f;
        }
        else if (data.input.was_key_pressed(GLFW_KEY_D))
        {
            t.scale_y *= 0.5f;
        }
        if (data.input.was_key_pressed(GLFW_KEY_W))
        {
            t.scale_y *= 0.5f;
            t.scale_x *= 0.5f;
        }
        else if (data.input.was_key_pressed(GLFW_KEY_S))
        {
            t.scale_y *= .3333f;
            t.scale_x *= .3333f;
        }

        //
        if (data.input.was_key_released(GLFW_KEY_A))
        {
            t.scale_x *= 2.f;
        }
        else if (data.input.was_key_released(GLFW_KEY_D))
        {
            t.scale_y *= 2.f;
        }
        if (data.input.was_key_released(GLFW_KEY_W))
        {
            t.scale_y *= 2.f;
            t.scale_x *= 2.f;
        }
        else if (data.input.was_key_released(GLFW_KEY_S))
        {
            t.scale_y *= 3.f;
            t.scale_x *= 3.f;
        }

        data.renderer.draw_scene(data.scene);
        glfwSwapBuffers(data.window);

        glfwPollEvents();
        data.input.update();

        data.limiter.wait_remainder();
        data.timer.end();

        print_frametime_debounce();
    }
}

GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height)
{
    GLFWwindow *window;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "RunShoot", nullptr, nullptr);
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
