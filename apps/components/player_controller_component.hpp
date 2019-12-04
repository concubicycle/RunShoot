//
// Created by sava on 11/10/19.
//

#ifndef __PLAYER_CONTROLLER_COMPONENT_HPP_
#define __PLAYER_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>
#include "turn_direction.hpp"

enum player_state
{
    running,
    airborne,
    turning,
    sliding,
    dying,
    dead
};

class player_controller_component : public ecs::component<player_controller_component>
{
public:
    glm::vec3 previous_direction {0.f, 0.f, -1.f};
    glm::vec3 direction { 0.f, 0.f, -1.f};
    glm::vec3 target_direction { 0.f, 0.f, -1.f };
    float total_turn_duration {0.5f}; // total time to turn
    float current_turn_duration {0.f}; // tune already spent turning
    turn_direction turn_dir;
    float previous_yaw {0};
    float target_yaw {0};
    std::int8_t turn_counter {0};
    float run_acceleration {0.03f};

    entity_id current_segment_id;
    int seg_clear_count {0};
    int segments_to_clear {2};

    glm::vec3 to_camera {0};
    float run_speed {5.f};
    float time_since_collision {0};
    float time_since_grounded {0};
    player_state state {running};
    float jump_force {0};
    float footstep_force {0};

    glm::vec2 recoil_acceleration {0.f};

    float gunshot_flash_time {0.1f};
    float time_to_flash_out { 0.1f};

    std::uint8_t footstep_sound_index = 1;

    float total_slide_duration { 1.f };
    float drop_speed { 5.f };
    float current_slide_duration {0.f};
    glm::vec3 original_to_camera {0.f};
    glm::vec3 target_to_camera {0.f};
    glm::vec3 slide_to_camera_offset {0.f, -5, 0.f};
    float down_t {0.2f};
    float up_t {0.8f};

    float fadeout_time {1.f};
    float current_fadeout_time {0.f};


    // 0...1 value used to interpolate between previous and target direction
    float turn_t()
    {
        return current_turn_duration / total_turn_duration;
    }

    // 0...1 value used to interpolate between previous and target direction
    float slide_t()
    {
        return current_slide_duration / total_slide_duration;
    }
};

void load_player_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__PLAYER_CONTROLLER_COMPONENT_HPP_
