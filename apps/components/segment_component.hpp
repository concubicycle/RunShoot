//
// Created by sava on 11/15/19.
//

#ifndef __SEGMENT_COMPONENT_HPP_
#define __SEGMENT_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <util/string_table.hpp>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include "turn_direction.hpp"

using nlohmann::json;

struct segment_component : public ecs::component<segment_component>
{
    glm::vec3 end_position {0.f};
    turn_direction turn {none};
};

void load_segment_component(const json& j, ecs::entity& e, string_table& hashes);

#endif //__SEGMENT_COMPONENT_HPP_
