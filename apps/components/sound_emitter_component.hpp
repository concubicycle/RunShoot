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

struct sound_emitter_component : ecs::component<sound_emitter_component>
{
    float volume { 1.f };
    bool enable_doppler {false};

    std::string sounds[MAX_SOUNDS];
    size_t sound_path_hashes[MAX_SOUNDS];
    std::uint16_t sound_count {0};

    size_t sounds_to_play[MAX_SOUNDS_TO_PLAY];
    std::uint16_t sounds_to_play_count {0};
};

void load_sound_emitter(const json& j, ecs::entity& e, string_table& hashes);



#endif //__SOUND_EMITTER_COMPONENT_HPP_
