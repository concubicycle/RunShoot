//
// Created by sava on 11/15/19.
//

#include "segment_component.hpp"
#include "runshoot_component.hpp"

void load_segment_component(const json& j, ecs::entity& e, string_table& hashes)
{
    auto& seg = e.get_component<segment_component>();
    seg.end_position = {
        j["end_position"][0].get<float>(),
        j["end_position"][1].get<float>(),
        j["end_position"][2].get<float>(),
    };

    seg.turn = j["turn"].get<turn_direction>();
}


template<> const component_shift ecs::component<segment_component>::component_bitshift =
    runshoot_component::Segment;
