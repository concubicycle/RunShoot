//
// Created by sava on 11/27/19.
//

#ifndef __MUSIC_PLAYER_COMPONENT_HPP_
#define __MUSIC_PLAYER_COMPONENT_HPP_

#include <string>

#define MAX_NUM_TRACKS 16

struct music_player_component
{
    std::string tracks[MAX_NUM_TRACKS];
    std::uint16_t track_count;


};


#endif //__MUSIC_PLAYER_COMPONENT_HPP_
