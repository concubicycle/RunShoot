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
        explicit sphere_collider(bool is_trigger = false) : collider(is_trigger) {}

        explicit sphere_collider(sphere shape, bool is_trigger = false) :
            collider(is_trigger),
            _shape(shape),
            _shape_original(shape) {}

        contact visit(aabb &box, glm::vec3 &combined_velocity) override
        {
            auto v = -combined_velocity;
            return intersect(box, _shape, v);
        }

        contact visit(sphere &sphere, glm::vec3 &combined_velocity) override
        {
            return intersect(_shape, sphere, combined_velocity);
        }

        contact accept(shape_visitor &visitor, glm::vec3 &combined_velocity) override
        {
            return visitor.visit(_shape, combined_velocity);
        }

        std::optional<float> intersect_ray(const ray& r) override
        {
            return std::optional<float>();
        }

        void set_position(glm::vec3& position) override
        {
            _shape.center = _shape_original.center + position;
        }

        void set_transform(glm::mat4& transform) override
        {
            _shape.center = transform * glm::vec4(_shape_original.center, 1);
        }

        sphere &shape() { return _shape; }

    private:
        sphere _shape;
        sphere _shape_original;
    };

}

#endif //__SPHERE_COLLIDER_HPP_
