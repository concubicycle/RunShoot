//
// Created by sava on 10/18/19.
//

#include "milestone1.hpp"


void handle_input(
    ecs::entity& entity,
    core::input_manager& input,
    float frame_time)
{
    auto speed = 5.f;

    auto& t = entity.get_component<ecs::transform_component>();

    //
    if (input.is_key_down(GLFW_KEY_A))
    {
        t.x -= frame_time * speed;
    }
    else if (input.is_key_down(GLFW_KEY_D))
    {
        t.x += frame_time * speed;
    }
    if (input.is_key_down(GLFW_KEY_W))
    {
        t.y += frame_time * speed;
    }
    else if (input.is_key_down(GLFW_KEY_S))
    {
        t.y -= frame_time * speed;
    }

    //
    if (input.was_key_pressed(GLFW_KEY_A))
    {
        t.scale_x *= 0.5f;
    }
    else if (input.was_key_pressed(GLFW_KEY_D))
    {
        t.scale_y *= 0.5f;
    }
    if (input.was_key_pressed(GLFW_KEY_W))
    {
        t.scale_y *= 0.5f;
        t.scale_x *= 0.5f;
    }
    else if (input.was_key_pressed(GLFW_KEY_S))
    {
        t.scale_y *= .3333f;
        t.scale_x *= .3333f;
    }

    //
    if (input.was_key_released(GLFW_KEY_A))
    {
        t.scale_x *= 2.f;
    }
    else if (input.was_key_released(GLFW_KEY_D))
    {
        t.scale_y *= 2.f;
    }
    if (input.was_key_released(GLFW_KEY_W))
    {
        t.scale_y *= 2.f;
        t.scale_x *= 2.f;
    }
    else if (input.was_key_released(GLFW_KEY_S))
    {
        t.scale_y *= 3.f;
        t.scale_x *= 3.f;
    }
}


void spin_quad( ecs::entity& entity,
                float frame_time)
{
    auto angular_v = 3.14f / 3.f;

// rotate entity. TODO: Implement behaviors somehow
    auto& t = entity.get_component<ecs::transform_component>();
    t.yaw += (angular_v * frame_time);

    if (t.yaw >  6.2831853)
        t.yaw = 0;
}