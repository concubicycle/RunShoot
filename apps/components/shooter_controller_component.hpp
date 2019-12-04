//
// Created by sava on 12/3/19.
//

#ifndef __SHOOTER_CONTROLLER_COMPONENT_HPP_
#define __SHOOTER_CONTROLLER_COMPONENT_HPP_

#include <ecs/entity.hpp>
#include <ecs/component.hpp>
#include <nlohmann/json.hpp>
#include <util/string_table.hpp>

using nlohmann::json;



struct shooter_controller_component : ecs::component<shooter_controller_component>
{
    enum shooter_state
    {
        waiting,
        shooting,
        dying
    };

    shooter_state last_state {waiting};
    shooter_state state {waiting};

    float shoot_interval {2.f};
    float time_to_shoot {2.f};

    float shot_flash_time {0.15f};
    float remaining_shot_flash_time {0.15f};
    bool did_shoot {false};

    float time_to_die {5.f};

    float kill_chance = {0.06f};
};

void load_shooter_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__SHOOTER_CONTROLLER_COMPONENT_HPP_
