//
// Created by sava on 11/25/19.
//

#ifndef __RAY_HPP_
#define __RAY_HPP_

#include <glm/vec3.hpp>

namespace physics_models
{
    struct ray
    {
        glm::vec3 start;
        glm::vec3 direction;
    };
}

#endif //__RAY_HPP_
