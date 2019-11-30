//
// Created by sava on 11/28/19.
//

#ifndef __SOUND_EMITTER_COMPONENT_HPP_
#define __SOUND_EMITTER_COMPONENT_HPP_

#include <ecs/ecs_types.hpp>
#include <ecs/component.hpp>
#include <ecs/entity.hpp>

#include <nlohmann/json.hpp>
#include <util/string_table.hpp>

#define MAX_SOUNDS 16
#define MAX_SOUNDS_TO_PLAY 8

using nlohmann::json;

enum sound_state
{
    uninitialized = 0,
    unchanged = 1,
    started,
    playing,
    stopped
};

struct sound_emitter_component : ecs::component<sound_emitter_component>
{
    float volume { 1.f };
    bool enable_doppler {false};

    std::string sounds[MAX_SOUNDS];
    size_t sound_path_hashes[MAX_SOUNDS];
    std::uint16_t sound_count {0};

    sound_state sound_states[MAX_SOUNDS];
    std::uint32_t playback_ids[MAX_SOUNDS_TO_PLAY];
};

void load_sound_emitter(const json& j, ecs::entity& e, string_table& hashes);



#endif //__SOUND_EMITTER_COMPONENT_HPP_
