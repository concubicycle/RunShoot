//
// Created by sava on 10/20/19.
//

#ifndef __BEHAVIOR_CONTEXT_HPP_
#define __BEHAVIOR_CONTEXT_HPP_

#include "frame_timer.hpp"
#include "input_manager.hpp"

namespace core
{
    struct behavior_context
    {
        frame_timer& time;
        input_manager& input;
    };
}
#endif //__BEHAVIOR_CONTEXT_HPP_
