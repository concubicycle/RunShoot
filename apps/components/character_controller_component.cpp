//
// Created by sava on 10/20/19.
//

#include <ecs/components/basic_components.hpp>
#include "character_controller_component.hpp"


template<> const std::uint8_t ecs::component<character_controller_component>::component_bitshift = MAX_BASIC_COMPONENTS+1;

void load_character_controller(const json& j, ecs::entity& e)
{
    auto& c = e.get_component<character_controller_component>();
    c.run_speed = j["run_speed"].get<float>();
    c.walk_speed = j["walk_speed"].get<float>();
}