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
        explicit aabb_collider(bool is_trigger = false) : collider(is_trigger), _position(0) {}

        explicit aabb_collider(aabb shape, bool is_trigger = false) :
            collider(is_trigger),
            _shape(shape),
            _position(0) {}

        contact visit(aabb &aabb, glm::vec3 &combined_velocity) override
        {
            return intersect(aabb, _shape, combined_velocity);
        }

        contact visit(sphere &sphere, glm::vec3 &combined_velocity) override
        {
            return intersect(_shape, sphere, combined_velocity);
        }

        contact accept(shape_visitor &visitor, glm::vec3 &combined_velocity) override
        {
            return visitor.visit(_shape, combined_velocity);
        }

        void set_position(glm::vec3& position) override
        {
            auto translate = position - _position;
            _position = position;

            _shape.min += translate;
            _shape.max += translate;
        }

        aabb &shape() { return _shape; }

    private:
        aabb _shape;
        glm::vec3 _position;
    };
}


#endif //__AABB_COLLIDER_HPP_
