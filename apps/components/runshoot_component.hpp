//
// Created by sava on 11/10/19.
//

#ifndef __RUNSHOOT_COMPONENT_HPP_
#define __RUNSHOOT_COMPONENT_HPP_

#include <ecs/components/basic_components.hpp>


enum runshoot_component
{
    FreeflyCamera = MAX_BASIC_COMPONENTS + 1, // 17
    DroneController = MAX_BASIC_COMPONENTS + 2, // 18
    PlayerController = MAX_BASIC_COMPONENTS + 3, // 19
    Segment = MAX_BASIC_COMPONENTS + 4, // 20
    TurnTrigger = MAX_BASIC_COMPONENTS + 5 // 21
};

#endif //__RUNSHOOT_COMPONENT_HPP_
