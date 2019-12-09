//
// Created by sava on 12/8/19.
//

#ifndef __MAIN_MENU_CONTROLLER_COMPONENT_HPP_
#define __MAIN_MENU_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>

#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>

using nlohmann::json;

struct main_menu_controller_component : public ecs::component<main_menu_controller_component>
{
    float foo;
    float bar;
};

void load_main_menu_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__MAIN_MENU_CONTROLLER_COMPONENT_HPP_
