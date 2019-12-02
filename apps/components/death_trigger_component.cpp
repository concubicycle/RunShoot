//
// Created by sava on 12/2/19.
//

#include "death_trigger_component.hpp"
#include "runshoot_component.hpp"


template<> const std::uint8_t ecs::component<death_trigger_component>::component_bitshift =
    runshoot_component::DeathTrigger;


void load_death_trigger(const json& j, ecs::entity& e, string_table& hashes)
{

}