//
// Created by sava on 12/3/19.
//

#include "shooter_controller_component.hpp"
#include "runshoot_component.hpp"

template<> const component_shift ecs::component<shooter_controller_component>::component_bitshift =
    runshoot_component::Shooter;


void load_shooter_controller(const json& j, ecs::entity& e, string_table& hashes)
{

}

