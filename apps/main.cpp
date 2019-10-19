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

#include "milestone1.hpp"

int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf.width(), conf.height(), conf);

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
    auto& entity2 = data.scene.entity_world().get_entity(456);

    debounce print_frametime_debounce(float_seconds(1.f), [&data](){
        std::cout << std::endl << "========" << std::endl << data.timer.frame_info() << std::endl;
    });

    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();
        auto frame_time = data.timer.smoothed_delta_secs();

        handle_input(entity, data.input, frame_time);
        spin_quad(entity, frame_time);
        spin_quad(entity2, frame_time);

        data.renderer.draw_scene(data.scene);
        glfwSwapBuffers(data.window);

        glfwPollEvents();
        data.input.update();

        data.limiter.wait_remainder();
        data.timer.end();

        print_frametime_debounce();
    }
}

