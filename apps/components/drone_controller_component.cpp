//
// Created by sava on 10/29/19.
//

#include <nlohmann/json.hpp>
using nlohmann::json;

#include <ecs/components/basic_components.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>
#include "drone_controller_component.hpp"

template<> const std::uint8_t ecs::component<drone_controller_component>::component_bitshift = MAX_BASIC_COMPONENTS+2;

void load_drone_controller(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& c = e.get_component<drone_controller_component>();
    c.acceleration = j["acceleration"].get<float>();
    c.flash_frequency = j["flash_frequency"].get<float>();
    c.state = (drone_controller_component::drone_state) j["state"].get<int>();
}