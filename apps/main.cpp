#define _M3D_H_

/////////////
#include <GLFW/glfw3.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>

using namespace gl;
using namespace glbinding;
using GLenum = gl::GLenum;
//////////////

#include <chrono>

#include <asset/texture_manager.hpp>
#include <asset/basic_mesh_reader.hpp>
#include <asset/scene_loader.hpp>
#include <asset/assimp_loader.hpp>
#include <asset/prototype_loader.hpp>

#include <core/frame_limiter.hpp>
#include <core/frame_timer.hpp>
#include <core/input_manager.hpp>
#include <core/startup_config.hpp>
#include <core/system_info.hpp>
#include <ecs/entity_factory.hpp>
#include <ecs/entity_world.hpp>
#include <events/event_exchange.hpp>
#include <file_read_std.h>
#include <renderer/model_render_loader.hpp>
#include <renderer/renderer.hpp>

#include "freefly_controller.hpp"
#include "components/add_custom_components.hpp"
#include "runshoot.hpp"
#include "player_controller.hpp"

using float_seconds = std::chrono::duration<float>;


void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int);


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

    asset::basic_mesh_reader reader;
    asset::assimp_loader assimp_reader;
    asset::texture_manager textures(app_string_table);
    asset::component_loader component_loader(app_string_table);
    asset::prototype_loader prototypes;
    asset::scene_loader loader(events, component_loader, prototypes);

    core::system_info info;
    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);
    core::input_manager input(window);

    rendering::shader_set shaders;
    rendering::model_render_loader render_loader(shaders, reader, assimp_reader, app_string_table, textures);
    rendering::renderer renderer(conf, info, shaders, events);

    ecs::entity_factory factory(1);
    ecs::entity_world entities(factory, events);

    physics::collisions collisions;
    physics::physics_world physics(events, collisions);

    freefly_controller controller(events);
    drone_controller drone_controller(events);
    player_controller player_controller(events);

    auto scene = loader.load_scene("./assets/scenes/runshoot_gameplay.json", entities);

    renderer.init();
    glfwSetFramebufferSizeCallback(window, build_framebuffer_callback(renderer));

    render_loader.init_entity_world_render_components(entities);
    textures.unload_all();

    game_systems data = {window, timer, limiter, input, renderer, scene, events, physics};
    behaviors behaviors = {controller, drone_controller, player_controller};
    render_loop(data, behaviors);
}

void render_loop(game_systems &data, behaviors &behaviors)
{
    auto &player = data.scene.entity_world().get_entity(111);

    core::behavior_context ctx = {data.timer, data.input};

    while (!glfwWindowShouldClose(data.window))
    {
        data.timer.start();
        data.physics.update(data.timer.delta_secs());
        data.events.update(data.timer.smoothed_delta());

        behaviors.character.update(ctx);
        behaviors.drone.update(ctx);
        behaviors.player.update(ctx);

        data.renderer.draw_scene(data.scene);

        glfwSwapBuffers(data.window);

        glfwPollEvents();
        data.input.update();

        data.limiter.wait_remainder();
        data.timer.end();
    }
}


void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int)
{
    static rendering::renderer& renderer = r;

    void (*callback)(GLFWwindow*, int, int) = ([](GLFWwindow* window, int width, int height) {
        renderer.resize(width, height);
    });

    return callback;
}


