//
// Created by sava on 10/17/19.
//

#ifndef __RUNSHOOT_HPP_
#define __RUNSHOOT_HPP_

#include <GLFW/glfw3.h>
#include <core/frame_timer.hpp>
#include <core/input_manager.hpp>
#include <renderer/renderer.hpp>
#include <asset/scene.hpp>
#include <core/frame_limiter.hpp>
#include <physics/physics_world.hpp>
#include "freefly_controller.hpp"
#include "drone_controller.hpp"
#include "player_controller.hpp"

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
};

struct behaviors
{
    freefly_controller& character;
    drone_controller& drone;
    player_controller& player;
};


GLFWwindow *set_up_glfw(core::startup_config& config);
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(game_systems &data, behaviors& behaviors);

void (*build_framebuffer_callback(rendering::renderer& r))(GLFWwindow*, int, int);



#endif //__RUNSHOOT_HPP_
