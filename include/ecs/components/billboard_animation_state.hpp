//
// Created by sava on 12/3/19.
//

#ifndef __BILLBOARD_ANIMATION_STATE_HPP_
#define __BILLBOARD_ANIMATION_STATE_HPP_

#include <ecs/component.hpp>
#include <glm/gtc/type_precision.hpp>



namespace ecs
{

    struct billboard_animation_state
    {
        static const unsigned int MaxAnimationFrames = 256;
        glm::u32vec2 frames[MaxAnimationFrames];
        std::uint32_t frame_count {0};
    };
}
#endif //__BILLBOARD_ANIMATION_STATE_HPP_
