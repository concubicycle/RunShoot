//
// Created by sava on 10/21/19.
//

#ifndef __CHARACTER_CONTROLLER_HPP_
#define __CHARACTER_CONTROLLER_HPP_

#include <iostream>
#include <core/behavior.hpp>
#include <events/event_exchange.hpp>

#include <glm/gtx/euler_angles.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include "components/character_controller_component.hpp"


class character_controller : public core::behavior
{
public:
    explicit character_controller(events::event_exchange& events)  : behavior(events) {}

    component_bitset required_components()  const override
    {
        return
            character_controller_component::archetype_bit |
            ecs::transform_component::archetype_bit |
            ecs::camera_component::archetype_bit;
    }

protected:

    void update_single(ecs::entity& e, core::behavior_context &ctx) override
    {
        auto& t = e.get_component<ecs::transform_component>();
        auto& c = e.get_component<ecs::camera_component>();
        auto& ctr = e.get_component<character_controller_component>();

        auto& input = ctx.input;
        auto frame_time = ctx.time.smoothed_delta_secs();
        auto speed = ctr.walk_speed;

        auto mouse_delta = input.mouse_delta();

        t.pitch -= mouse_delta.y * 0.1f * frame_time;
        t.yaw += mouse_delta.x * 0.1f * frame_time;
        t.roll = 0;

        auto half_pi = glm::pi<float>() / 2.f;
        if (t.pitch > half_pi)
            t.pitch = half_pi;
        if (t.pitch < -half_pi)
            t.pitch = -half_pi;

        c.pitch = t.pitch;
        c.yaw = t.yaw;
        c.roll = t.roll;

        float cosp = cosf(c.pitch);
        float sinp = sinf(c.pitch);
        float siny = sinf(c.yaw);
        float cosy = cosf(c.yaw);

        glm::vec3 fwd(siny * cosp, sinp, -cosp * cosy);
        glm::vec3 up(-siny * sinp, cosp, sinp * cosy);
        glm::vec3 right = glm::cross(fwd, up);

        //
        if (input.is_key_down(GLFW_KEY_W))
        {
            t.pos += fwd * frame_time * speed;
        }
        if (input.is_key_down(GLFW_KEY_A))
        {
            t.pos -= right * frame_time * speed;
        }
        else if (input.is_key_down(GLFW_KEY_D))
        {
            t.pos += right * frame_time * speed;
        }

        else if (input.is_key_down(GLFW_KEY_S))
        {
            t.pos -= fwd * frame_time * speed;
        }

        c.position = t.pos;

        std::cout << glm::to_string(t.pos) << std::endl;
    }
};


#endif //__CHARACTER_CONTROLLER_HPP_
