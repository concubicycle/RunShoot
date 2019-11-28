//
// Created by sava on 11/28/19.
//

#ifndef __MUSIC_PLAYER_HPP_
#define __MUSIC_PLAYER_HPP_
#include <iostream>

#include <chrono>
#include <thread>

#include <fmod.hpp>
#include <fmod_errors.h>

#include <ecs/ecs_types.hpp>
#include <core/behavior.hpp>

#include "components/music_player_component.hpp"


class music_player : public core::behavior
{
public:
    explicit music_player(events::event_exchange& events);

    component_bitset required_components()  const override
    {
        return music_player_component::archetype_bit;
    }

    void update_single(ecs::entity &e, core::behavior_context &ctx) override;

    void on_entity_created(ecs::entity &e) override;



private:
    FMOD::System *_system = nullptr;
    FMOD::ChannelGroup *_channelGroup = nullptr;
};


#endif //__MUSIC_PLAYER_HPP_
