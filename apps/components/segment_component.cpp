//
// Created by sava on 11/15/19.
//

#include "segment_component.hpp"
#include "runshoot_component.hpp"

void load_segment_component(const json& j, ecs::entity& e, string_table& hashes)
{}


template<> const component_shift ecs::component<segment_component>::component_bitshift =
    runshoot_component::Segment;
