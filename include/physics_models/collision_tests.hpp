//
// Created by sava on 11/6/19.
//

#ifndef __COLLISION_TESTS_HPP_
#define __COLLISION_TESTS_HPP_

#include <glm/vec3.hpp>

#include "aabb.hpp"
#include "contact.hpp"
#include "sphere.hpp"

namespace physics_models
{
    // for combined_velocity, consider
    contact intersect(aabb& c0, aabb& c1, glm::vec3& combined_velocity);
    contact intersect(aabb& c0, sphere& c1, glm::vec3& combined_velocity);
    contact intersect(sphere& c0, sphere& c1, glm::vec3& combined_velocity);
}

#endif //__COLLISION_TESTS_HPP_