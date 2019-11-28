//
// Created by sava on 11/28/19.
//

#include "music_player.hpp"


#include <iostream>

#include <chrono>
#include <thread>

#include <fmod.hpp>
#include <fmod_errors.h>


bool succeededOrWarn(const std::string &message, FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        std::cerr << message << ": " << result << " " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}

FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL *channelControl,
                                            FMOD_CHANNELCONTROL_TYPE controlType,
                                            FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
                                            void *commandData1, void *commandData2)
{
    std::cout << "Callback called for " << controlType << std::endl;
    return FMOD_OK;
}


music_player::music_player(events::event_exchange &events) : behavior(events)
{
    FMOD_RESULT result;

    // Create the main system object.
    result = FMOD::System_Create(&_system);
    if (!succeededOrWarn("FMOD: Failed to create system object", result))
        throw "Failed to init fmod";

    // Initialize FMOD.
    result = _system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (!succeededOrWarn("FMOD: Failed to initialise system object", result))
        throw "Failed to init fmod";

    // Create the channel group.
    result = _system->createChannelGroup("inGameSoundEffects", &_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to create in-game sound effects channel group", result))
        throw "Failed to init fmod";
}


void music_player::update_single(ecs::entity &e, core::behavior_context &ctx)
{

}

void music_player::on_entity_created(ecs::entity &e)
{
    FMOD_RESULT result;
    auto &component = e.get_component<music_player_component>();

    if (component.track_count == 0) return;

    auto track = component.tracks[0];

    // Create the sound.
    FMOD::Sound *sound = nullptr;
    _system->createSound(track.c_str(), FMOD_LOOP_NORMAL, nullptr, &sound);

    // Play the sound.
    FMOD::Channel *channel = nullptr;
    result = _system->playSound(sound, nullptr, false, &channel);
    if (!succeededOrWarn("FMOD: Failed to play sound", result))
        throw "Failed to init fmod";

    // Assign the channel to the group.
    result = channel->setChannelGroup(_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to set channel group on", result))
        throw "Failed to init fmod";

    channel->setCallback(&channelGroupCallback);
    if (!succeededOrWarn("FMOD: Failed to set callback for sound", result))
        throw "Failed to init fmod";


}
