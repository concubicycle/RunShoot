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

struct game_systems
{
    GLFWwindow *window;
    core::frame_timer &timer;
    core::frame_limiter &limiter;
    core::input_manager& input;
    rendering::renderer& renderer;
    asset::scene& scene;
};


GLFWwindow *set_up_glfw(std::uint32_t width, std::uint32_t height, core::startup_config& config);
void run_game(core::startup_config &conf, GLFWwindow *window);
void render_loop(game_systems &data);


#endif //__RUNSHOOT_HPP_
