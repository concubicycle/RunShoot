//
// Created by sava on 12/8/19.
//

#include "main_menu_controller_component.hpp"
#include "runshoot_component.hpp"

template<> const component_shift ecs::component<main_menu_controller_component>::component_bitshift =
    runshoot_component::MainMenu;

void load_main_menu_controller(const json& j, ecs::entity& e, string_table& hashes)
{

}
