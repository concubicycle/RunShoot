//
// Created by sava on 11/18/19.
//

#include "turn_trigger_component.hpp"
#include "runshoot_component.hpp"


template<> const std::uint8_t ecs::component<turn_trigger_component>::component_bitshift =
    runshoot_component::TurnTrigger;

void load_turn_trigger(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& c = e.get_component<turn_trigger_component>();
    c.direction = (turn_direction) j["direction"].get<int>();
}
