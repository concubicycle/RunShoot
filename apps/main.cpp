#include <chrono>

#include <core/frame_limiter.hpp>
#include <core/frame_timer.hpp>
#include <core/startup_config.hpp>
#include <core/system_info.hpp>
#include <core/input_manager.hpp>


#include <asset/texture_manager.hpp>
#include <asset/basic_mesh_reader.hpp>
#include <asset/scene_loader.hpp>
#include <asset/assimp_loader.hpp>
#include <asset/prototype_loader.hpp>


#include <ecs/entity_factory.hpp>
#include <ecs/entity_world.hpp>
#include <events/event_exchange.hpp>
#include <file_read_std.h>
#include <renderer/model_render_loader.hpp>
#include <renderer/renderer.hpp>
#include <renderer/debug_drawer.hpp>
#include <sound_wrapper/sound_system.hpp>
#include <animation/texture_animator.hpp>

#include "freefly_controller.hpp"
#include "components/add_custom_components.hpp"
#include "runshoot.hpp"
#include "player_controller.hpp"
#include "drone_spawner.hpp"
#include "music_player.hpp"


/////////////
#include <GLFW/glfw3.h>
//////////////


using float_seconds = std::chrono::duration<float>;


void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int);


int main()
{
	std::srand(std::time(0)); //use current time as seed for random generator

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
    asset::prototype_spawner spawner(events, component_loader, prototypes);
    asset::scene_loader loader(spawner);

    core::system_info info;
    core::frame_timer timer;
    core::frame_limiter limiter(timer, 60);
    core::input_manager input(window);

    rendering::shader_set shaders;
    rendering::model_render_loader render_loader(shaders, reader, assimp_reader, app_string_table, textures, events);
    rendering::renderer renderer(conf, info, shaders, events);
    rendering::debug_drawer debug_draw(conf, info, events, shaders, input);

    ecs::entity_factory factory(1);
    ecs::entity_world entities(factory, events);

    physics::collisions collisions;
    physics::physics_world physics(events, collisions);

    sound::sound_system game_sound(app_string_table, events);

    animation::texture_animator billboard_animation(events);

    freefly_controller controller(events);
    drone_controller drone_controller(events);
    player_controller player_controller(events);
    segment_spawner segment_spawn(events);
    drone_spawner drone_spawn(events);
    music_player music(events, app_string_table, game_sound);
    shooter_controller shooter(events);

	asset::scene scene(spawner, entities);
    loader.load_scene("./assets/scenes/runshoot_gameplay.json", entities, scene);

    renderer.init();
    glfwSetFramebufferSizeCallback(window, build_framebuffer_callback(renderer));

    game_systems data = {window, timer, limiter, input, renderer, scene, events, physics, debug_draw, game_sound, billboard_animation};
    behaviors behaviors = {controller, drone_controller, player_controller, segment_spawn, drone_spawn, music, shooter};
    render_loop(data, behaviors);
}

void render_loop(game_systems &systems, behaviors &behaviors)
{
    auto &player = systems.scene.entity_world().get_entity(111);

    core::behavior_context ctx = {systems.timer, systems.input, systems.scene, systems.physics, systems.renderer };

    while (!glfwWindowShouldClose(systems.window))
    {
        systems.timer.start();
        systems.physics.update(systems.timer.delta_secs());
        systems.events.update(systems.timer.smoothed_delta());

        behaviors.character.update(ctx);
        behaviors.drone.update(ctx);
        behaviors.player.update(ctx);
        behaviors.segment_spawn.update(ctx);
        behaviors.drone_spawn.update(ctx);
        behaviors.music.update(ctx);
        behaviors.shooter.update(ctx);

        systems.renderer.draw_scene(systems.scene);
        systems.debug_draw.update();
        systems.game_sound.update();
        systems.billboard_animation.update(systems.timer.smoothed_delta_secs());

        glfwSwapBuffers(systems.window);

        glfwPollEvents();
        systems.input.update();

        systems.limiter.wait_remainder();
        systems.timer.end();

        if (systems.input.was_key_pressed(GLFW_KEY_ESCAPE)) break;
    }
}


void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int)
{
    static rendering::renderer& r_ref = r;

    void (*callback)(GLFWwindow*, int, int) = ([](GLFWwindow* window, int width, int height) {
        rendering::renderer::resize(width, height);
    });

    return callback;
}


