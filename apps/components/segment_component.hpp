//
// Created by sava on 11/15/19.
//

#ifndef __SEGMENT_COMPONENT_HPP_
#define __SEGMENT_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <util/string_table.hpp>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>

using nlohmann::json;

class segment_component : public ecs::component<segment_component>
{
public:

};

void load_segment_component(const json& j, ecs::entity& e, string_table& hashes);

#endif //__SEGMENT_COMPONENT_HPP_
