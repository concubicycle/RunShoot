//
// Created by sava on 10/20/19.
//

#ifndef __EVENT_TYPE_HPP_
#define __EVENT_TYPE_HPP_

#include <cstdint>

namespace events
{
    enum event_type : std::uint32_t
    {
        entity_created = 1,
        entity_destroyed = 2,
        collision = 3,

        delay_entity_jump = 4,
        camera_prop_set = 5,
        scene_change = 6,
        exit = 7
    };
}

#endif //__EVENT_TYPE_HPP_
