//
// Created by sava on 10/28/19.
//

#ifndef __AABB_HPP_
#define __AABB_HPP_

#include <glm/vec2.hpp>

namespace physics_models
{

    struct aabb
    {
        glm::vec3 min;
        glm::vec3 max;
    };
}

#endif //__AABB_HPP_
