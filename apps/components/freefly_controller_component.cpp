//
// Created by sava on 10/20/19.
//

#include <ecs/components/basic_components.hpp>
#include "freefly_controller_component.hpp"
#include "runshoot_component.hpp"

template<> const std::uint8_t ecs::component<freefly_controller_component>::component_bitshift =
    runshoot_component::FreeflyCamera;

void load_character_controller(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& c = e.get_component<freefly_controller_component>();
    c.run_speed = j["run_speed"].get<float>();
    c.walk_speed = j["walk_speed"].get<float>();
}
