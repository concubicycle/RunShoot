//
// Created by sava on 11/6/19.
//

#ifndef __SPHERE_COLLIDER_HPP_
#define __SPHERE_COLLIDER_HPP_

#include "collider.hpp"
#include <physics_models/collision_tests.hpp>

namespace physics_models
{

    class sphere_collider : public collider
    {
    public:
        sphere_collider() {}

        sphere_collider(sphere shape) : _shape(shape) {}

        contact visit(aabb &box, glm::vec3 &combined_velocity)
        {
            auto v = -combined_velocity;
            return intersect(box, _shape, v);
        }

        contact visit(sphere &sphere, glm::vec3 &combined_velocity)
        {
            return intersect(_shape, sphere, combined_velocity);
        }

        contact accept(shape_visitor &visitor, glm::vec3 &combined_velocity)
        {
            return visitor.visit(_shape, combined_velocity);
        }

        void set_position(glm::vec3& position)
        {
            _shape.center = position;
        }

        sphere &shape() { return _shape; }

    private:
        sphere _shape;
    };

}

#endif //__SPHERE_COLLIDER_HPP_
