//
// Created by sava on 11/21/19.
//

#ifndef __SEGMENT_SPAWNER_COMPONENT_HPP_
#define __SEGMENT_SPAWNER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <util/string_table.hpp>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>

struct segment_spawner_component : ecs::component<segment_spawner_component>
{
    std::uint32_t num_initial_segments {5};
    float turn_frequency {0.3f};

    glm::vec4 current_direction {0, 0, -1, 0};
    glm::vec3 current_position;
    float jump_distance { 90.f };
    float jump_distance_variance {30.f};

    glm::mat4 rotation {1.f};
};

void load_segment_spawner(const json& j, ecs::entity& e, string_table& hashes);

#endif //__SEGMENT_SPAWNER_COMPONENT_HPP_
