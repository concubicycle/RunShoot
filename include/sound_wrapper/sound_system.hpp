//
// Created by sava on 11/30/19.
//

#ifndef __SOUND_SYSTEM_HPP_
#define __SOUND_SYSTEM_HPP_

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <fmod.hpp>
#include <util/string_table.hpp>
#include <ecs/entity.hpp>
#include <atomic>
#include <events/listener.hpp>
#include <events/event_exchange.hpp>
#include "sound_emitter_component.hpp"

namespace sound
{

    class sound_system
    {
    public:
        sound_system(string_table &app_strings, events::event_exchange& events);

        ~sound_system();

        playback_id play_sound_3d(size_t path_hash, bool loop);
        playback_id play_sound_one_off(size_t path_hash, bool loop);


        void stop_sound(playback_id);

        void update();

        void stop_all();

    private:
        events::event_exchange& _events;

        FMOD::ChannelGroup *_channelGroup = nullptr;
        FMOD::System *_system = nullptr;
        std::unordered_map<playback_id, std::pair<FMOD::Sound *, FMOD::Channel *>> _playbacks;
        std::unordered_map<size_t, FMOD::Sound *> _sounds;
        std::unordered_map<size_t, playback_id> _sound_to_playback;
        std::vector<std::reference_wrapper<ecs::entity>> _emitters;
        std::vector<std::reference_wrapper<ecs::entity>> _listeners;

        string_table &_app_strings;

        std::atomic_uint _next_playback_id = 1;

        listener_id _entity_create_listener_id;
        listener_id _entity_destroy_listener_id;

        void grab_entity (ecs::entity &e);
        void forget_entity (ecs::entity &e);

        void sync_transform_to_listener();
        void update_emitters();

        FMOD::Sound *get_sound(size_t hash, FMOD_MODE mode);
        playback_id play_sound(FMOD::Sound * sound, size_t path_hash);

    };


}

#endif //__SOUND_SYSTEM_HPP_
