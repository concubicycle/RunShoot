//
// Created by sava on 11/28/19.
//

#ifndef __SOUND_HPP_
#define __SOUND_HPP_

#include <unordered_map>
#include <fmod.hpp>
#include <util/string_table.hpp>

class sound
{
public:
    sound(string_table& app_strings);
    ~sound();

    void play_sound(size_t path_hash, bool loop);


private:
    std::unordered_map<size_t, FMOD::Sound*> _sounds;

    FMOD::System *_system = nullptr;
    FMOD::ChannelGroup *_channelGroup = nullptr;

    string_table& _app_strings;

    FMOD::Sound* get_sound(size_t hash, bool loop);
};


#endif //__SOUND_HPP_
