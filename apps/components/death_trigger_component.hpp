//
// Created by sava on 12/2/19.
//

#ifndef __DEATH_TRIGGER_COMPONENT_HPP_
#define __DEATH_TRIGGER_COMPONENT_HPP_

#include <nlohmann/json.hpp>
using nlohmann::json;

#include <ecs/component.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>

struct death_trigger_component : ecs::component<death_trigger_component>
{
    float foo;
    float bar;
    float baz;

};

void load_death_trigger(const json& j, ecs::entity& e, string_table& hashes);


#endif //__DEATH_TRIGGER_COMPONENT_HPP_
