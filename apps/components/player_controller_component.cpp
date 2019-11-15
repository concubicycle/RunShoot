//
// Created by sava on 11/10/19.
//

#include "player_controller_component.hpp"
#include "runshoot_component.hpp"
#include <ecs/ecs_types.hpp>

template<> const component_shift ecs::component<player_controller_component>::component_bitshift =
    runshoot_component::PlayerController;


void load_player_controller(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& c = e.get_component<player_controller_component>();
    c.run_speed = j["run_speed"].get<float>();
    c.jump_force = j["jump_force"].get<float>();
    c.footstep_force = j["footstep_force"].get<float>();
    c.total_turn_duration = j["total_turn_duration"].get<float>();
}

