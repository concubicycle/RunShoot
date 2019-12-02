//
// Created by sava on 10/17/19.
//

#ifndef __RUNSHOOT_HPP_
#define __RUNSHOOT_HPP_


#include <core/frame_timer.hpp>
#include <core/input_manager.hpp>
#include <renderer/renderer.hpp>
#include <asset/scene.hpp>
#include <core/frame_limiter.hpp>
#include <physics/physics_world.hpp>
#include <renderer/debug_drawer.hpp>
#include "freefly_controller.hpp"
#include "drone_controller.hpp"
#include "player_controller.hpp"
#include "segment_spawner.hpp"
#include "drone_spawner.hpp"
#include "music_player.hpp"

#if (WIN32)
#define NOMINMAX
#include <windows.h>
#endif

#include <GLFW/glfw3.h>

struct game_systems
{
    GLFWwindow *window;
    core::frame_timer &timer;
    core::frame_limiter &limiter;
    core::input_manager& input;
    rendering::renderer& renderer;
    asset::scene& scene;
    events::event_exchange& events;
    physics::physics_world& physics;
    rendering::debug_drawer& debug_draw;
    sound::sound_system& game_sound;
};

struct behaviors
{
    freefly_controller& character;
    drone_controller& drone;
    player_controller& player;
    segment_spawner& segment_spawn;
    drone_spawner& drone_spawn;
    music_player& music;
};


GLFWwindow *set_up_glfw(core::startup_config& config);
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(game_systems &systems, behaviors& behaviors);

void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int);



#endif //__RUNSHOOT_HPP_
