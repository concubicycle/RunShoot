//
// Created by sava on 10/29/19.
//

#ifndef __DRONE_CONTROLLER_COMPONENT_HPP_
#define __DRONE_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>

struct drone_controller_component : public ecs::component<drone_controller_component>
{
    enum drone_state
    {
        following = 1,
        swooping = 2,
        exploding = 3,
        flashing = 4
    };

    drone_state state;

    float acceleration;
    glm::vec3 target {0.f, 15.f, 0.f};
    float friction {0.5f};

    float horizontal_offset { 0.f};
    float vertical_offset { 0.f};
    float z_offset { 0.f};

    ecs::entity* player_ptr { nullptr };

    float time_to_flash { 3.f };
    float initial_time_to_flash { 3.f };
    float initial_warning_light_duration {1.f};
    float warning_light_duration {1.f};
    float initial_warning_light_interval { 1.f };
    float warning_light_interval {1.f};
    float time_to_warning_light {0.f};
    std::int8_t initial_flashes_before_blind {8};
    std::int8_t flashes_before_blind {8};
    bool light_on {false};
};

void load_drone_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__DRONE_CONTROLLER_COMPONENT_HPP_
