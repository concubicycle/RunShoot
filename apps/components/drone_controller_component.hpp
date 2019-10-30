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

    // temp for milestone 2 - fly up/down
    float time_to_dir_switch = 0;
    float dir_switch_interval = 0.75f;
    int dir = 10;

};

void load_drone_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__DRONE_CONTROLLER_COMPONENT_HPP_
