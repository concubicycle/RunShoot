//
// Created by sava on 11/28/19.
//

#ifndef __MUSIC_PLAYER_HPP_
#define __MUSIC_PLAYER_HPP_
#include <iostream>

#include <chrono>
#include <thread>

#include <ecs/ecs_types.hpp>
#include <ecs/behavior.hpp>
#include <sound_wrapper/sound_system.hpp>

#include "components/music_player_component.hpp"
#include <ecs/behavior.hpp>


class music_player : public ecs::behavior<core::behavior_context>
{
public:
    explicit music_player(events::event_exchange& events, string_table& app_strings, sound::sound_system& sound);

    component_bitset required_components()  const override
    {
        return music_player_component::archetype_bit;
    }

    void update_single(ecs::entity &e, core::behavior_context &ctx) override;

    void on_entity_created(ecs::entity &e) override;


private:
    string_table& _app_strings;
    sound::sound_system& _sound;
};


#endif //__MUSIC_PLAYER_HPP_
