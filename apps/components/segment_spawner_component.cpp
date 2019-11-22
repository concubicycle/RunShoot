//
// Created by sava on 11/21/19.
//

#include "segment_spawner_component.hpp"
#include "runshoot_component.hpp"


void load_segment_spawner(const json& j, ecs::entity& e, string_table& hashes)
{

}


template<> const component_shift ecs::component<segment_spawner_component>::component_bitshift =
    runshoot_component::SegmentSpawner;
