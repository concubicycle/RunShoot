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

        std::optional<float> intersect_ray(const ray& r) override
        {
            float t_min = 0;
            float t_max =  std::numeric_limits<float>::max();

            for (int i = 0; i < 3; i++)
            {
                float min = _shape.min[i];
                float max = _shape.max[i];
                float d_ray = r.direction[i];
                float p_ray = r.start[i];

                auto not_moving = glm::abs(d_ray) < glm::epsilon<float>();
                auto outside = p_ray < min || p_ray > max;

                if (not_moving && outside)
                    return std::optional<float>();

                if (p_ray < min && d_ray < 0) return std::optional<float>();
                if (p_ray > max && d_ray > 0) return std::optional<float>();

                float ood = 1.f / d_ray;
                float t1 = (min - p_ray) * ood;
                float t2 = (max - p_ray) * ood;

                if (t1 > t2)
                {
                    float dummy = t1;
                    t1 = t2;
                    t2 = dummy;
                }

                t_min = std::max(t_min, t1);
                t_max = std::min(t_max, t2);

                if (t_min > t_max) return std::optional<float>();
            }

            return t_min;
        }

        void set_position(glm::vec3& position) override
        {
            _shape.min = _shape_original.min + position;
            _shape.max = _shape_original.max + position;
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
