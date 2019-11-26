//
// Created by sava on 11/5/19.
//

#ifndef __COLLIDER_HPP_
#define __COLLIDER_HPP_

#include "contact.hpp"
#include "sphere.hpp"
#include "ray.hpp"

#include <glm/vec3.hpp>

#include <physics_models/collision_tests.hpp>

namespace physics_models
{
    class shape_visitor
    {
    public:
        virtual contact visit(aabb &collider, glm::vec3 &combined_velocity) = 0;

        virtual contact visit(sphere &collider, glm::vec3 &combined_velocity) = 0;
    };

    class shape_element
    {
    public:
        virtual contact accept(shape_visitor &visitor, glm::vec3 &combined_velocity) = 0;
    };

    class collider : public shape_element, public shape_visitor
    {
    public:
        explicit collider(bool is_trigger) : _is_trigger(is_trigger) {}

        virtual void set_position(glm::vec3& position) = 0;
        virtual void set_transform(glm::mat4& transform) = 0;

        virtual std::optional<float> intersect_ray(const ray& r) = 0;

        [[nodiscard]] bool is_trigger() const { return _is_trigger; }

    private:
        bool _is_trigger;
    };
}

#endif //__COLLIDER_HPP_
