//
// Created by sava on 10/29/19.
//

#ifndef __DRONE_CONTROLLER_HPP_
#define __DRONE_CONTROLLER_HPP_

#include <glm/gtc/random.hpp>

#include <core/behavior.hpp>
#include "components/drone_controller_component.hpp"
#include "components/player_controller_component.hpp"

class drone_controller : public core::behavior
{
public:
    explicit drone_controller(events::event_exchange &events) : behavior(events) {}

    component_bitset required_components() const override
    {
        return
            drone_controller_component::archetype_bit |
            ecs::transform_component::archetype_bit;
    }


protected:

    void update_single(ecs::entity &e, core::behavior_context &ctx) override
    {
        auto &rb = e.get_component<ecs::rigid_body_component>();
        auto &component = e.get_component<drone_controller_component>();
        auto &transform = e.get_component<ecs::transform_component>();


        auto frame_time = ctx.time.smoothed_delta_secs();

        set_target(e);
        thrust_toward_target(e);
        friction(rb, component);
    }

    void thrust_toward_target(ecs::entity &e)
    {
        auto &rb = e.get_component<ecs::rigid_body_component>();
        auto &drone = e.get_component<drone_controller_component>();
        auto to_target = drone.target - rb.position;

        if (to_target.y < 0) to_target.y = 0.1f;
        to_target.y *= 2;
        to_target.x *= .65;
        to_target.z *= .65;

        rb.force += to_target * drone.acceleration;
    }

    void friction(ecs::rigid_body_component &rb, drone_controller_component &drone)
    {
        // up the friction when close to target
        auto to_target = drone.target - rb.position;
        auto tangent = glm::normalize(glm::cross(glm::vec3(0, 1, 0), to_target));
        float friction = drone.friction + std::min((1 - drone.friction) * 0.7f, 1.f / to_target.length());


        rb.force += -(rb.velocity) * friction;

        // cancel off some tangent velocity, try not to orbit the player
        float tan_speed = glm::dot(rb.velocity, tangent);

        if (tan_speed * tan_speed > 100.f)
            rb.force -= 1.5f * tan_speed * tangent;
    }

private:

    void set_target(ecs::entity &e)
    {
        auto &rb = e.get_component<ecs::rigid_body_component>();
        auto &drone = e.get_component<drone_controller_component>();
        if (!drone.player_ptr)
        {
            drone.target = glm::vec3(0.f, 15.f, 0.f);
            return;
        }

        auto &player = drone.player_ptr->get_component<player_controller_component>();
        auto &player_t = drone.player_ptr->get_component<ecs::transform_component>();
        drone.target = player_t.pos + player.direction * 50.f;
        drone.target.y = 20.f;

        auto to_player = player_t.pos - rb.position;
        drone.target += glm::normalize(glm::cross(to_player, glm::vec3(0, 1, 0))) * drone.horizontal_offset;
        drone.target += glm::vec3(0, 1, 0) * drone.vertical_offset;
        drone.target += player.direction * drone.z_offset;

        float d_sq = glm::length2(drone.target - rb.position);

        if (d_sq < 4.f)
        {
            auto offset = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
            offset.y *= 0.5f;
            drone.target += offset;
        }
    }
};

#endif //__DRONE_CONTROLLER_HPP_
