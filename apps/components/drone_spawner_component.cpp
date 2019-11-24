//
// Created by sava on 11/23/19.
//

#include "drone_spawner_component.hpp"
#include "runshoot_component.hpp"

void load_drone_spawner(const json& j, ecs::entity& e, string_table& hashes)
{

}

template<> const std::uint8_t ecs::component<drone_spawner_component>::component_bitshift =
    runshoot_component::DroneSpawner;

