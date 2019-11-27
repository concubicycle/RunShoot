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
    float turn_frequency {0.35f};

    glm::vec4 current_direction {0, 0, -1, 0};
    glm::vec3 current_position;
    float jump_distance { 80.f };
    float jump_distance_variance {30.f};

    std::uint8_t num_initial_segments {10};
    std::uint8_t initial_straights {3};

    glm::mat4 rotation {1.f};
};

void load_segment_spawner(const json& j, ecs::entity& e, string_table& hashes);

#endif //__SEGMENT_SPAWNER_COMPONENT_HPP_
