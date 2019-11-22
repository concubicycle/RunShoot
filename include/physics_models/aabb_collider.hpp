//
// Created by sava on 11/6/19.
//

#ifndef __AABB_COLLIDER_HPP_
#define __AABB_COLLIDER_HPP_

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include "sphere.hpp"
#include "contact.hpp"
#include "aabb.hpp"
#include "collider.hpp"


namespace physics_models
{

    class aabb_collider : public collider
    {
    public:
        explicit aabb_collider(bool is_trigger = false) : collider(is_trigger) {}

        explicit aabb_collider(aabb shape, bool is_trigger = false) :
            collider(is_trigger),
            _shape(shape),
            _shape_original(shape) {}

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
            _shape.min += _shape_original.min + position;
            _shape.max += _shape_original.max + position;
        }

        void set_transform(glm::mat4& transform) override
        {
            glm::vec3 min = transform * glm::vec4(_shape_original.min, 1);
            glm::vec3 max = transform * glm::vec4(_shape_original.max, 1);

            _shape.min.x = glm::min(min.x, max.x);
            _shape.min.y = glm::min(min.y, max.y);
            _shape.min.z = glm::min(min.z, max.z);

            _shape.max.x = glm::max(min.x, max.x);
            _shape.max.y = glm::max(min.y, max.y);
            _shape.max.z = glm::max(min.z, max.z);

//            std::cout << glm::to_string(_shape.min) << std::endl;
//            std::cout << glm::to_string(_shape.max) << std::endl << std::endl;
        }

        aabb &shape() { return _shape; }

    private:
        aabb _shape;
        aabb _shape_original;
    };
}


#endif //__AABB_COLLIDER_HPP_
