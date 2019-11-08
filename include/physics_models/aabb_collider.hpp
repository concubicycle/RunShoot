//
// Created by sava on 11/6/19.
//

#ifndef __AABB_COLLIDER_HPP_
#define __AABB_COLLIDER_HPP_

#include <glm/vec3.hpp>
#include "sphere.hpp"
#include "contact.hpp"
#include "aabb.hpp"
#include "collider.hpp"


namespace physics_models
{

    class aabb_collider : public collider
    {
    public:
        aabb_collider() {}

        aabb_collider(aabb shape) : _shape(shape), _half_span((_shape.max - _shape.min) / 2.f) {}

        contact visit(aabb &aabb, glm::vec3 &combined_velocity)
        {
            return intersect(aabb, _shape, combined_velocity);
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
            _shape.min = position - _half_span;
            _shape.max = position + _half_span;
        }

        aabb &shape() { return _shape; }

    private:
        aabb _shape;
        glm::vec3 _half_span;
    };

}


#endif //__AABB_COLLIDER_HPP_