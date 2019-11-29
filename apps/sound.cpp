//
// Created by sava on 11/28/19.
//

#include <string>
#include <fmod_errors.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include "sound.hpp"

bool succeededOrWarn(const std::string &message, FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        spdlog::error("{0}: {1} {3}", message, result, FMOD_ErrorString(result));
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

sound::sound(string_table& strings) : _app_strings(strings)
{
    FMOD_RESULT result;

    // Create the main system object.
    result = FMOD::System_Create(&_system);
    if (!succeededOrWarn("FMOD: Failed to create system object", result))
        throw;

    // Initialize FMOD.
    result = _system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (!succeededOrWarn("FMOD: Failed to initialise system object", result))
        throw;

    // Create the channel group.
    result = _system->createChannelGroup("inGameSoundEffects", &_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to create in-game sound effects channel group", result))
        throw;
}


sound::~sound()
{
    spdlog::info("Releasing FMOD items");

    for (auto& pair : _sounds)
        pair.second->release();

    _channelGroup->release();
    _system->release();
}


void sound::play_sound(size_t path_hash, bool loop)
{
    FMOD_RESULT result;
    auto sound = get_sound(path_hash, loop);

    // Play the sound.
    FMOD::Channel *channel = nullptr;
    result = _system->playSound(sound, nullptr, false, &channel);
    if (!succeededOrWarn("FMOD: Failed to play sound", result))
        throw;

    // Assign the channel to the group.
    result = channel->setChannelGroup(_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to set channel group on", result))
        throw;

    channel->setCallback(&channelGroupCallback);
    if (!succeededOrWarn("FMOD: Failed to set callback for sound", result))
        throw;
}

FMOD::Sound *sound::get_sound(size_t hash, bool loop)
{
    if (_sounds.find(hash) == _sounds.end())
    {
        // Create the sound.
        FMOD::Sound *sound = nullptr;
        auto& track = _app_strings[hash];

        auto mode = loop ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
        _system->createSound(track.c_str(),  mode, nullptr, &sound);

        _sounds.insert({hash, sound});
    }

    return _sounds.find(hash)->second;
}
