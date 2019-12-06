//
// Created by sava on 11/21/19.
//

#ifndef __SEGMENT_SPAWNER_COMPONENT_HPP_
#define __SEGMENT_SPAWNER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <util/string_table.hpp>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include "turn_direction.hpp"

struct segment_spawner_component : ecs::component<segment_spawner_component>
{
    float turn_frequency {0.4f};

    glm::vec4 current_direction {0, 0, -1, 0};
    glm::vec3 current_position;
    float jump_distance { 80.f };
    float jump_distance_variance {30.f};

    std::uint8_t num_initial_segments {10};

    glm::mat4 rotation {1.f};

    glm::uint32_t segments_before_shooters {2};

    std::uint32_t last_two_turns[2] {0, 0};

    bool are_last_two_turns(std::uint32_t dir)
    {
        return last_two_turns[0] == dir && last_two_turns[1] == dir;
    }
};

void load_segment_spawner(const json& j, ecs::entity& e, string_table& hashes);

#endif //__SEGMENT_SPAWNER_COMPONENT_HPP_
