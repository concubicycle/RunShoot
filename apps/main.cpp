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
#include <core/system_info.hpp>
#include <renderer/renderer.hpp>
#include <asset/basic_mesh_reader.hpp>
#include <file_read_std.h>
#include <asset/texture_manager.hpp>
#include <ecs/entity_factory.hpp>
#include <ecs/entity_world.hpp>
#include <asset/scene_loader.hpp>
#include <chrono>

using float_seconds = std::chrono::duration<float>;

#include "runshoot.hpp"

#include "milestone1.hpp"

#include <events/event_exchange.hpp>
#include <renderer/model_render_loader.hpp>

#include "components/add_custom_components.hpp"
#include "character_controller.hpp"

int main()
{
    core::startup_config conf = core::startup_config();
    conf.load();

    auto window = set_up_glfw(conf);

    if (window == nullptr)
        return -1;

    run_game(conf, window);

    glfwTerminate();
    spdlog::shutdown();

    return 0;
}


void run_game(core::startup_config &conf, GLFWwindow *window)
{
    add_custom_components();

    string_table app_string_table;

    events::event_exchange events;

    character_controller controller(events);

    asset::basic_mesh_reader reader;
    asset::assimp_loader assimp_reader;
    asset::texture_manager textures(app_string_table);
    asset::scene_loader loader(app_string_table);

    core::system_info info;
    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);
    core::input_manager input(window);

    rendering::shader_set shaders;
    rendering::model_render_loader render_loader(shaders, reader, assimp_reader, app_string_table, textures);
    rendering::renderer renderer(conf, info, shaders, events);

    ecs::entity_factory factory(1);
    ecs::entity_world entities(factory, events);

    auto scene = loader.load_scene("./assets/scenes/scene.json", entities);
    renderer.init();
    render_loader.init_entity_world_render_components(entities);
    textures.unload_all();

    game_systems data = {window, timer, limiter, input, renderer, scene, events};
    behaviors behaviors = {controller};

    render_loop(data, behaviors);
}

void render_loop(game_systems &data, behaviors &behaviors)
{
    auto &cube1 = data.scene.entity_world().get_entity(1);
    auto &cube2 = data.scene.entity_world().get_entity(2);
    auto &player = data.scene.entity_world().get_entity(111);

    core::behavior_context ctx = {data.timer, data.input};

    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();
        auto frame_time = data.timer.smoothed_delta_secs();

        behaviors.character.update(ctx);

        spin_quad(cube1, frame_time);

        data.renderer.draw_scene(data.scene);
        glfwSwapBuffers(data.window);

        glfwPollEvents();
        data.input.update();

        data.limiter.wait_remainder();
        data.timer.end();
    }
}

