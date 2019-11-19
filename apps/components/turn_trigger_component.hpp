//
// Created by sava on 11/18/19.
//

#ifndef __TURN_TRIGGER_COMPONENT_HPP_
#define __TURN_TRIGGER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <ecs/ecs_types.hpp>

#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>
#include "turn_direction.hpp"

using nlohmann::json;



struct turn_trigger_component : ecs::component<turn_trigger_component>
{
    turn_direction direction;
};

void load_turn_trigger(const json& j, ecs::entity& e, string_table& hashes);


#endif //__TURN_TRIGGER_COMPONENT_HPP_
