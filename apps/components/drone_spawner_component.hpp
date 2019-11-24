//
// Created by sava on 11/23/19.
//

#ifndef __DRONE_SPAWNER_COMPONENT_HPP_
#define __DRONE_SPAWNER_COMPONENT_HPP_


#include <ecs/component.hpp>
#include "freefly_controller_component.hpp"

struct drone_spawner_component : public ecs::component<drone_spawner_component>
{
    float drone_spawn_interval { 10.f };
    float time_to_spawn {10.f};
    float horizontal_spawn_offset {0.f};
    float vertical_spawn_offset {30.f};
};

void load_drone_spawner(const json& j, ecs::entity& e, string_table& hashes);



#endif //__DRONE_SPAWNER_COMPONENT_HPP_
