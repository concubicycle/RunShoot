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
        exploding = 3
    };

    drone_state state;

    float acceleration;
    float flash_frequency;
    glm::vec3 target {0.f, 15.f, 0.f};
    float friction {0.5f};

    float horizontal_offset { 0.f};
    float vertical_offset { 0.f};
    float z_offset { 0.f};

    ecs::entity* player_ptr { nullptr };
};

void load_drone_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__DRONE_CONTROLLER_COMPONENT_HPP_
