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

using nlohmann::json;


namespace sound
{
    typedef std::uint32_t playback_id;


    enum sound_state
    {
        uninitialized = 0,
        unchanged = 1,
        started,
        playing,
        stopped
    };

    struct emitter_sound
    {
        std::string path;
        size_t path_hash;
        sound_state state {uninitialized};
        playback_id playback {0};
        bool loop {false};
        float volume {1.f};
    };

    struct sound_emitter_component : ecs::component<sound_emitter_component>
    {
        std::uint16_t sound_count{0};
        emitter_sound emitter_sounds[MAX_SOUNDS];
        void add_sound(std::string path, size_t hash)
        {
            emitter_sounds[sound_count++] = {
                path,
                hash
            };
        }

        void set_sound_state(std::uint32_t index, sound_state state)
        {
            emitter_sounds[index].state = state;
        }
    };

    void load_sound_emitter(const json &j, ecs::entity &e, string_table &hashes);
}



#endif //__SOUND_EMITTER_COMPONENT_HPP_
